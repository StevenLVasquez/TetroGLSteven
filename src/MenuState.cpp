#include "MenuState.h"
#include "PlayState.h"
#include "HighScoreState.h"
#include "StateManager.h"
#include "Image.h"

CMenuState::CMenuState(CStateManager* pManager)
	: CGameState(pManager), m_pFont(NULL), m_iCurrentSelection(0),
	m_pCurrentGame(NULL)
{
	m_pFont = new CGameFont;
	m_pFont->CreateFont("Times New Roman", 30, FW_NORMAL);

	// Ajustar estas dimensiones según tu pantalla o la imagen de fondo
	int screenWidth = 1920;
	int screenHeight = 1080;

	// Crear las imágenes del menú
	m_pBackgroundImg = CImage::CreateImage("Logotetris.png", TRectanglei(0, screenHeight, 0, screenWidth));
	m_pTitleImg = CImage::CreateImage("Logotetris.png", TRectanglei(0, 300, 0, 500));
	m_pItemBckgndNormal = CImage::CreateImage("MenuItems.png", TRectanglei(0, 57, 0, 382));
	m_pItemBckgndSelected = CImage::CreateImage("MenuItems.png", TRectanglei(58, 114, 0, 382));

	// Crear los textos para cada opción del menú
	m_pNewGameText = new CTextControl(m_pFont, TRectanglei(150, 207, 209, 591));
	m_pNewGameText->SetAlignement(CTextControl::TACenter);
	m_pNewGameText->SetText("Jugar");

	m_pResumeGameText = new CTextControl(m_pFont, TRectanglei(250, 307, 209, 591));
	m_pResumeGameText->SetAlignement(CTextControl::TACenter);
	m_pResumeGameText->SetText("Volver al Juego");

	m_pScoresText = new CTextControl(m_pFont, TRectanglei(350, 407, 209, 591));
	m_pScoresText->SetAlignement(CTextControl::TACenter);
	m_pScoresText->SetText("Puntuacion");

	m_pExitText = new CTextControl(m_pFont, TRectanglei(450, 507, 209, 591));
	m_pExitText->SetAlignement(CTextControl::TACenter);
	m_pExitText->SetText("Salir Del Juego");

	// Ahora definimos los rectángulos para detectar el clic del ratón.
	// Observando el Draw(): los ítems se dibujan a partir de x=209 y y=150, con incrementos de 100 en Y.
	// Suponiendo que el ancho del ítem es 382 y la altura es 57 (según las coords en el TRectanglei de las imágenes):

	int itemWidth = 382;
	int itemHeight = 57;
	int startX = 209;
	int startY = 150;

	m_rectNewGame = TRectanglei(startY, startY + itemHeight, startX, startX + itemWidth);      // Jugar (150, 150+57, 209, 209+382)
	m_rectResumeGame = TRectanglei(startY + 100, startY + 100 + itemHeight, startX, startX + itemWidth); // Volver al Juego (250, 250+57, 209, 209+382)
	m_rectScores = TRectanglei(startY + 200, startY + 200 + itemHeight, startX, startX + itemWidth); // Puntuacion (350, 350+57, 209, 209+382)
	m_rectExit = TRectanglei(startY + 300, startY + 300 + itemHeight, startX, startX + itemWidth); // Salir (450, 450+57, 209, 209+382)
}

CMenuState::~CMenuState()
{
}

CMenuState* CMenuState::GetInstance(CStateManager* pManager)
{
	static CMenuState Instance(pManager);
	return &Instance;
}

void CMenuState::OnKeyDown(WPARAM wKey)
{
	switch (wKey)
	{
	case VK_DOWN:
		SelectionDown();
		break;
	case VK_UP:
		SelectionUp();
		break;
	case VK_RETURN:
		SelectionChosen();
		break;
	}
}

void CMenuState::OnLButtonDown(int x, int y)
{
	if (RectContainsPoint(m_rectNewGame, x, y))
	{
		m_iCurrentSelection = 0;
		SelectionChosen();
	}
	else if (RectContainsPoint(m_rectResumeGame, x, y))
	{
		m_iCurrentSelection = 1;
		SelectionChosen();
	}
	else if (RectContainsPoint(m_rectScores, x, y))
	{
		m_iCurrentSelection = 2;
		SelectionChosen();
	}
	else if (RectContainsPoint(m_rectExit, x, y))
	{
		m_iCurrentSelection = 3;
		SelectionChosen();
	}
}

bool CMenuState::RectContainsPoint(const TRectanglei& rect, int x, int y)
{
	return (x >= rect.m_Left && x <= rect.m_Right && y >= rect.m_Top && y <= rect.m_Bottom);
}

void CMenuState::Draw()
{
	m_pBackgroundImg->BlitImage();
	m_pTitleImg->BlitImage();
	// Dibujar los items del menú
	for (int i = 0; i < 4; i++)
	{
		if (i == m_iCurrentSelection)
			m_pItemBckgndSelected->BlitImage(209, 150 + i * 100);
		else
			m_pItemBckgndNormal->BlitImage(209, 150 + i * 100);
	}

	m_pNewGameText->Draw();
	m_pResumeGameText->Draw();
	m_pScoresText->Draw();
	m_pExitText->Draw();
}

void CMenuState::EnterState()
{
	// Revisar si hay un juego actual activo
	m_iCurrentSelection = 0;
	if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
		m_pResumeGameText->SetTextColor(0.5, 0.5, 0.5);
	else
		m_pResumeGameText->SetTextColor(1.0, 1.0, 1.0);
}

void CMenuState::SelectionUp()
{
	m_iCurrentSelection--;
	if (m_iCurrentSelection == -1)
		m_iCurrentSelection = 3;

	if (m_iCurrentSelection == 1)
	{
		if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
			m_iCurrentSelection--;
	}
}

void CMenuState::SelectionDown()
{
	m_iCurrentSelection++;
	if (m_iCurrentSelection == 4)
		m_iCurrentSelection = 0;

	if (m_iCurrentSelection == 1)
	{
		if (!m_pCurrentGame || m_pCurrentGame->IsGameOver())
			m_iCurrentSelection++;
	}
}

void CMenuState::SelectionChosen()
{
	switch (m_iCurrentSelection)
	{
	case 0:
		if (!m_pCurrentGame)
			m_pCurrentGame = CPlayState::GetInstance(m_pStateManager);
		m_pCurrentGame->Reset();
		ChangeState(m_pCurrentGame);
		break;

	case 1:
		if (m_pCurrentGame && !m_pCurrentGame->IsGameOver())
			ChangeState(m_pCurrentGame);
		break;

	case 2:
		ChangeState(CHighScoreState::GetInstance(m_pStateManager));
		break;

	case 3:
		PostQuitMessage(0);
		break;
	}
}
