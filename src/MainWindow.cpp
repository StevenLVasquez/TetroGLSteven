#include "MainWindow.h"
#include "Exception.h"
#include "PlayState.h"
#include "MenuState.h"
#include "GameFont.h"
#include <Windows.h>  
#include <windowsx.h> 
using namespace std;

#define WINDOW_CLASSNAME    "TetroGLSTT"  // Window class name

CMainWindow::CMainWindow(int iWidth, int iHeight, bool bFullScreen)
	: m_hWindow(NULL), m_hDeviceContext(NULL), m_hGLContext(NULL),
	m_bFullScreen(bFullScreen)
{
	RegisterWindowClass();

	RECT WindowRect;
	WindowRect.top = WindowRect.left = 0;
	WindowRect.right = iWidth;
	WindowRect.bottom = iHeight;

	DWORD dwExStyle = 0;
	DWORD dwStyle = 0;

	if (m_bFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = iWidth;
		dmScreenSettings.dmPelsHeight = iHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN)
			!= DISP_CHANGE_SUCCESSFUL)
		{
			throw CException("Unable to swith to fullscreen mode");
		}

		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	m_hWindow = CreateWindowEx(dwExStyle, TEXT(WINDOW_CLASSNAME),
		TEXT("TetroGL"),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
		0, 0, WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL, NULL,
		GetModuleHandle(NULL),
		this);
	if (m_hWindow == NULL)
		throw CException("Cannot create the main window");

	CreateContext();
	InitGL();
	ShowWindow(m_hWindow, SW_SHOW);
	OnSize(iWidth, iHeight);

	CGameFont::SetDeviceContext(m_hDeviceContext);

	m_pStateManager = new CStateManager;
	m_pStateManager->ChangeState(CMenuState::GetInstance(m_pStateManager));
}

CMainWindow::~CMainWindow()
{
	if (m_bFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if (m_hGLContext)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	if (m_hDeviceContext)
	{
		ReleaseDC(m_hWindow, m_hDeviceContext);
		m_hDeviceContext = NULL;
	}

	DestroyWindow(m_hWindow);
	UnregisterClass(TEXT(WINDOW_CLASSNAME), GetModuleHandle(NULL));
}

LRESULT CMainWindow::OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_NCCREATE)
	{
		CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		SetWindowLongPtr(Handle, GWLP_USERDATA,
			reinterpret_cast<long>(pCreateStruct->lpCreateParams));
	}

	CMainWindow* pWindow = reinterpret_cast<CMainWindow*>
		(GetWindowLongPtr(Handle, GWLP_USERDATA));
	if (pWindow)
		pWindow->ProcessEvent(Message, wParam, lParam);

	return DefWindowProc(Handle, Message, wParam, lParam);
}

void CMainWindow::ProcessEvent(UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		OnSize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		m_pStateManager->OnKeyDown(wParam);
		break;
	case WM_KEYUP:
		m_pStateManager->OnKeyUp(wParam);
		break;
	case WM_CHAR:
		m_pStateManager->OnChar(wParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
}

void CMainWindow::OnLButtonDown(int x, int y)
{
	if (m_pStateManager)
		m_pStateManager->OnLButtonDown(x, y);
}

void CMainWindow::RegisterWindowClass()
{
	WNDCLASS WindowClass;
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = &CMainWindow::OnEvent;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = 0;
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = WINDOW_CLASSNAME;

	RegisterClass(&WindowClass);
}

void CMainWindow::CreateContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	m_hDeviceContext = GetDC(m_hWindow);
	if (!m_hDeviceContext)
		throw CException("Unable to create rendering context");

	int PixelFormat;
	PixelFormat = ChoosePixelFormat(m_hDeviceContext, &pfd);
	if (!PixelFormat)
		throw CException("Unable to create rendering context");

	if (!SetPixelFormat(m_hDeviceContext, PixelFormat, &pfd))
		throw CException("Unable to create rendering context");

	m_hGLContext = wglCreateContext(m_hDeviceContext);
	if (!m_hGLContext)
		throw CException("Unable to create rendering context");

	if (!wglMakeCurrent(m_hDeviceContext, m_hGLContext))
		throw CException("Unable to create rendering context");
}

void CMainWindow::InitGL()
{
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void CMainWindow::OnSize(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void CMainWindow::Update(DWORD dwCurrentTime)
{
	m_pStateManager->Update(dwCurrentTime);
}

void CMainWindow::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pStateManager->Draw();
	SwapBuffers(m_hDeviceContext);
}
