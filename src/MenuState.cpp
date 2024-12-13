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

	int screenWidth = 1070;
	int screenHeight = 845;

	// Imagen de fondo
	m_pBackgroundImg = CImage::CreateImage("tetrisprueba.png", TRectanglei(300, 1920, 0, 1080));

	// Logo centrado, no lo movemos
	m_pTitleImg = CImage::CreateImage("Logotetris.png", TRectanglei(0, 125, 0, 350));

	// Ítems del menú (imagen normal y seleccionada)
	m_pItemBckgndNormal = CImage::CreateImage("MenuItems.png", TRectanglei(0, 57, 0, 382));
	m_pItemBckgndSelected = CImage::CreateImage("MenuItems.png", TRectanglei(58, 114, 0, 382));

	// Vamos a centrar los botones horizontalmente y moverlos más abajo.
	// Centrar horizontalmente: (1070 - 382)/2 = ~344
	// Baja la posición inicial del primer botón a y=300 (antes estaba en 150)
	// Cada botón está separado por 100 píxeles en Y.
	int itemWidth = 382;
	int itemHeight = 57;
	int itemStartX = 344;
	int itemStartY = 300;

	// Ajustar los TRectanglei de los textos para alinearse con las nuevas posiciones:
	// 1er ítem (Jugar): y desde 300 a 300+57=357, x desde 344 a 344+382=726
	m_pNewGameText = new CTextControl(m_pFont, TRectanglei(300, 357, 344, 726));
	m_pNewGameText->SetAlignement(CTextControl::TACenter);
	m_pNewGameText->SetText("Jugar");

	// 2do ítem (Volver al Juego): Y desde 400 a 457
	m_pResumeGameText = new CTextControl(m_pFont, TRectanglei(400, 457, 344, 726));
	m_pResumeGameText->SetAlignement(CTextControl::TACenter);
	m_pResumeGameText->SetText("Volver al Juego");

	// 3er ítem (Puntuacion): Y desde 500 a 557
	m_pScoresText = new CTextControl(m_pFont, TRectanglei(500, 557, 344, 726));
	m_pScoresText->SetAlignement(CTextControl::TACenter);
	m_pScoresText->SetText("Puntuacion");

	// 4to ítem (Salir Del Juego): Y desde 600 a 657
	m_pExitText = new CTextControl(m_pFont, TRectanglei(600, 657, 344, 726));
	m_pExitText->SetAlignement(CTextControl::TACenter);
	m_pExitText->SetText("Salir Del Juego");

	// Actualizar los rectángulos para las colisiones del ratón con las nuevas posiciones:
	m_rectNewGame = TRectanglei(itemStartY, itemStartY + itemHeight, itemStartX, itemStartX + itemWidth);
	m_rectResumeGame = TRectanglei(itemStartY + 100, itemStartY + 100 + itemHeight, itemStartX, itemStartX + itemWidth);
	m_rectScores = TRectanglei(itemStartY + 200, itemStartY + 200 + itemHeight, itemStartX, itemStartX + itemWidth);
	m_rectExit = TRectanglei(itemStartY + 300, itemStartY + 300 + itemHeight, itemStartX, itemStartX + itemWidth);
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
	// Dibujamos el fondo
	m_pBackgroundImg->BlitImage();

	// Dibujamos el logo centrado (sin moverlo)
	int logoWidth = 380;
	int logoHeight = 57;
	int screenWidth = 1070;
	int posX = (screenWidth - logoWidth) / 2;
	int posY = 50;
	m_pTitleImg->BlitImage(posX, posY);

	// Dibujamos los ítems del menú en las nuevas posiciones
	int itemStartX = 344;
	int itemStartY = 300;
	for (int i = 0; i < 4; i++)
	{
		int drawX = itemStartX;
		int drawY = itemStartY + i * 100;
		if (i == m_iCurrentSelection)
			m_pItemBckgndSelected->BlitImage(drawX, drawY);
		else
			m_pItemBckgndNormal->BlitImage(drawX, drawY);
	}

	// Dibujamos los textos
	m_pNewGameText->Draw();
	m_pResumeGameText->Draw();
	m_pScoresText->Draw();
	m_pExitText->Draw();
}

void CMenuState::EnterState()
{
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
