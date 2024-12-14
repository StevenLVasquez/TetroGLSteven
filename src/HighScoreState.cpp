#include "HighScoreState.h"
#include "MenuState.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

CHighScoreState::CHighScoreState(CStateManager* pManager)
    : CGameState(pManager), m_ulNewHighScore(0), m_bEnterName(false),
    m_pFont(NULL), m_iNameIndex(0), m_vecHighScores()
{
    m_pFont = new CGameFont;
    m_pFont->CreateFont("Times New Roman", 30, FW_NORMAL);
    m_pCurrentName[0] = '\0';

    // Imágenes de fondo y título
    m_pBackgroundImg = CImage::CreateImage("tetrisscorest1.jpg", TRectanglei(0, 1070, 0, 1080));
    m_pTitleImg = CImage::CreateImage("Vintage1.png", TRectanglei(0, 125, 0, 350));

    // Mantener las coordenadas originales de las sub-imágenes que funcionaban antes.
    m_pEntriesBckgndImg = CImage::CreateImage("HighScoreItems.png", TRectanglei(70, 99, 0, 600));
    m_pEnterNameImg = CImage::CreateImage("HighScoreItems.png", TRectanglei(0, 39, 0, 150));
    m_pEnterNameBackImg = CImage::CreateImage("HighScoreItems.png", TRectanglei(40, 69, 0, 350));
}

CHighScoreState::~CHighScoreState()
{
    if (m_pFont)
    {
        delete m_pFont;
        m_pFont = NULL;
    }
}

CHighScoreState* CHighScoreState::GetInstance(CStateManager* pManager)
{
    static CHighScoreState Instance(pManager);
    return &Instance;
}

void CHighScoreState::OnKeyDown(WPARAM wKey)
{
    if (m_bEnterName)
    {
        switch (wKey)
        {
        case VK_RETURN:
            // Al presionar ENTER, se guarda el nombre con el puntaje con el que se perdió
            AddNewScore(m_pCurrentName, m_ulNewHighScore);
            m_ulNewHighScore = 0;
            m_bEnterName = false;
            m_iNameIndex = 0;
            m_pCurrentName[0] = '\0';

            // Volver al menú tras guardar
            ChangeState(CMenuState::GetInstance(m_pStateManager));
            break;
        case VK_BACK:
            // Borrar un caracter
            if (m_iNameIndex > 0)
            {
                m_iNameIndex--;
                m_pCurrentName[m_iNameIndex] = '\0';
            }
            break;
        }
    }
    else
    {
        switch (wKey)
        {
        case VK_ESCAPE:
        case VK_RETURN:
            ChangeState(CMenuState::GetInstance(m_pStateManager));
            break;
        }
    }
}

void CHighScoreState::OnChar(WPARAM wChar)
{
    if (m_bEnterName && (m_iNameIndex < 25))
    {
        // Permitir solo caracteres alfabéticos
        if ((wChar >= 'A' && wChar <= 'Z') || (wChar >= 'a' && wChar <= 'z'))
        {
            m_pCurrentName[m_iNameIndex] = (char)wChar;
            m_iNameIndex++;
            m_pCurrentName[m_iNameIndex] = '\0';
        }
    }
}

void CHighScoreState::Draw()
{
    m_pBackgroundImg->BlitImage();
    m_pTitleImg->BlitImage(10, 20);

    int yPos = 150;
    for (THighScoreTable::iterator iter = m_vecHighScores.begin(); iter != m_vecHighScores.end(); ++iter)
    {
        m_pEntriesBckgndImg->BlitImage(100, yPos);

        CTextControl txtEntry(m_pFont, TRectanglei(yPos, yPos + 30, 120, 700));
        txtEntry.SetAlignement(CTextControl::TALeft);
        txtEntry.SetText(iter->strPlayer);
        txtEntry.Draw();

        stringstream ssScore;
        ssScore << iter->ulScore;
        txtEntry.SetAlignement(CTextControl::TARight);
        txtEntry.SetText(ssScore.str());
        txtEntry.Draw();

        yPos += 40;
    }

    // Si se debe ingresar el nombre
    if (m_bEnterName)
    {
        // Dibujar la barra como antes
        m_pEnterNameImg->BlitImage(50, 525);
        m_pEnterNameBackImg->BlitImage(230, 530);
        m_pFont->DrawText(m_pCurrentName, 250, 550, 1.0, 1.0, 1.0);
    }
}

void CHighScoreState::EnterState()
{
    // Cargar puntuaciones
    m_vecHighScores.clear();
    ifstream inputFile("HighScores.txt");
    if (!inputFile.fail())
    {
        while (!inputFile.eof())
        {
            HighScoreData newScore;
            inputFile >> newScore.strPlayer >> newScore.ulScore;
            if (inputFile.fail()) break;
            m_vecHighScores.push_back(newScore);
        }

        // Ordenar
        sort(m_vecHighScores.begin(), m_vecHighScores.end());
    }

    // Si tenemos un score con el que se perdió (m_ulNewHighScore > 0),
    // siempre pedimos el nombre para guardarlo.
    if (m_ulNewHighScore > 0)
    {
        m_bEnterName = true;
        m_iNameIndex = 0;
        m_pCurrentName[0] = '\0';
    }
    else
    {
        // Si no hay un score que guardar, el jugador simplemente puede volver al menú
        m_bEnterName = false;
    }
}

void CHighScoreState::SaveScores()
{
    ofstream outputFile("HighScores.txt");
    if (outputFile.fail())
        return;

    for (THighScoreTable::iterator iter = m_vecHighScores.begin(); iter != m_vecHighScores.end(); ++iter)
    {
        outputFile << iter->strPlayer << " " << iter->ulScore << "\n";
    }
}

void CHighScoreState::AddNewScore(const std::string& strName, ULONG ulScore)
{
    HighScoreData newData;
    newData.strPlayer = strName;
    newData.ulScore = ulScore;
    m_vecHighScores.push_back(newData);

    // Ordenar la tabla
    sort(m_vecHighScores.begin(), m_vecHighScores.end());

    // Limitar a 10
    while (m_vecHighScores.size() > 10)
        m_vecHighScores.pop_back();

    SaveScores();
}
