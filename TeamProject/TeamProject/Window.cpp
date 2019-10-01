#include "stdafx.h"
#include "Window.h"

#include "Program.h"
#include "resource.h"
//static 변수를 선언했으면 아래와 같이 반드시 cpp에서 초기화 해주어야 한다. 이유는 구글링 해보세요
WindowDesc Window::windowDesc;
Program* Window::program = nullptr;

/*********************************************************************************
## WIndow ## 
윈도우 클래스 생성자 
레지스터 등록 및 윈도우 창 생성
**********************************************************************************/
Window::Window()
{
	this->SetRegister();
	this->CreateWindowApi();
}
/*********************************************************************************
## ~Window ##
윈도우 클래스 소멸자
**********************************************************************************/
Window::~Window()
{
	//디스플레이 세팅 초기화 
	if (windowDesc.bFullScreen == true)
		ChangeDisplaySettings(NULL, 0);
	//윈메인 해제
	DestroyWindow(windowDesc.Handle);
	//레지스터 해제 
	UnregisterClass(windowDesc.AppName.c_str(), windowDesc.Instance);
}
/*********************************************************************************
## Run ##
실질적인 메인 루프
**********************************************************************************/
WPARAM Window::Run()
{
	MSG msg = { 0 };

	//=========Init Manager=================================
	this->CreateManager();
	//=======================================================
	if (Window::windowDesc.bToolMode == true)
	{
		
	}
	else
	{

	}
	Window::program = new Program();
	Window::program->Init();
	_TimeManager->Start();
	//============Main Loop===================================
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			this->UpdateManager();
			Window::program->Update();
			CameraManager::Get()->Update();
			Window::program->Render();
		}
	}
	//=======================================================
	Window::program->Release();
	SafeDelete(Window::program);
	//================Release Manager===============================
	this->DeleteManager();
	//==============================================================

	return msg.wParam;
}

/*********************************************************************************
## WndProc ##
메인 프록시 
**********************************************************************************/
LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc((UINT*)handle, message, wParam, lParam)) return true;
	switch (message)
	{
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(handle, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	if (message == WM_KEYDOWN || message == WM_MOUSEMOVE)
		Input::Get()->InputProc(message, wParam, lParam);
	if (message == WM_MOUSEWHEEL)
		CameraManager::Get()->CameraProc(message, wParam, lParam);

	return (DefWindowProc(handle, message, wParam, lParam));
}
/*********************************************************************************
## SetRegister ##
레지스터 등록
**********************************************************************************/
void Window::SetRegister()
{
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;																		//클래스 여분 메모리 설정
	wndClass.cbWndExtra = 0;																		//윈도우 여분 메모리 설정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);									//윈도우 백그라운드
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);													//윈도우 커서 모양
	wndClass.hIcon = LoadIcon(Window::windowDesc.Instance, MAKEINTRESOURCE(IDI_ICON2));				//윈도우 아이콘 모양
	wndClass.hInstance = windowDesc.Instance;														//윈도우 인스턴스 번호
	wndClass.lpfnWndProc = (WNDPROC)WndProc;														//윈도우 프로시져
	wndClass.lpszClassName = windowDesc.AppName.c_str();											//윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;																	//윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;														//윈도우 창 스타일

																									//윈도우 정보를 등록해준다
	RegisterClass(&wndClass);

	if (windowDesc.bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)windowDesc.Width;
		devMode.dmPelsHeight = (DWORD)windowDesc.Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}
}
/*********************************************************************************
## CreateWindowApi ##
윈메인 생성 및 등록
**********************************************************************************/
void Window::CreateWindowApi()
{
	RECT rect = { 0, 0, (LONG)windowDesc.Width, (LONG)windowDesc.Height };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)windowDesc.Width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)windowDesc.Height) / 2;

	windowDesc.Handle = CreateWindow(
		windowDesc.AppName.c_str(),
		windowDesc.AppName.c_str(),
		WS_OVERLAPPEDWINDOW,
		centerX - WinSizeX / 2,
		centerY - WinSizeY / 2,
		WinSizeX,
		WinSizeY,
		NULL,
		(HMENU)NULL,
		windowDesc.Instance,
		NULL);

	assert(windowDesc.Handle != NULL);

	windowDesc.Hdc = GetDC(windowDesc.Handle);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(windowDesc.Handle, NULL, centerX - WinSizeX / 2, centerY - WinSizeY / 2, WinSizeX, WinSizeY
		, SWP_NOZORDER | SWP_NOMOVE);

	MoveWindow
	(
		windowDesc.Handle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);

	ShowWindow(windowDesc.Handle, Window::windowDesc.command);
	//SetForegroundWindow(windowDesc.Handle);
	//SetFocus(windowDesc.Handle);

	ShowCursor(true);
}
/*********************************************************************************
## CreateManager ##
싱글톤 메니져들 생성 부분
**********************************************************************************/
void Window::CreateManager()
{
	DXRenderer::Create();
	ImGui::Create(Window::GetWindowDesc().Handle, _D3DDevice, _D3DDeviceContext);
	{
		ImGui::StyleColorsClassic();
	}
	SoundManager::Create();
	Input::Create();
	CameraManager::Create();
	Time::Create();
	ImageManager::Create();
	ObjectManager::Create();
	SceneManager::Create();
	DamageFontManager::Create();
	Database::Create();
	DataManager::Create();
}
/*********************************************************************************
## DeleteManager ##
싱글톤 메니져들 해제 부분
**********************************************************************************/
void Window::DeleteManager()
{
	DamageFontManager::Delete();
	SceneManager::Delete();
	ObjectManager::Delete();
	ImageManager::Delete();
	Time::Delete();
	CameraManager::Delete();
	Input::Delete();
	SoundManager::Delete();
	ImGui::Delete();
	DXRenderer::Delete();
	Database::Delete();
	DataManager::Delete();
}

/**********************************************************************************
## UpdateManager ##
메니져들 업데이트 
***********************************************************************************/
void Window::UpdateManager()
{
	Time::Get()->Update();
	SoundManager::Get()->Update();
	DamageFontManager::Get()->Update();
	Input::Get()->Update();
	ImGui::Update();

}
