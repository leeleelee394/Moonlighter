/*************************************************************
## WinMain.cpp ##
윈메인 입니다. 레지스터 등록 및 윈도우 창 생성은 Window클래스에 일임하고 
이곳에서는 생성할 윈도우 창 정보만 구조체에 초기화합니다. 
*************************************************************/
#include "stdafx.h"
#include "TeamProject.h"

bool _isDebug = false;

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	//윈도우 정보 구조체 
	WindowDesc desc;
	desc.AppName = L"ChanSoonHaRee";	//윈도우 창 이름
	desc.Instance = instance;			//윈도우 인스턴스
	desc.bFullScreen = false;			//전체화면 여부
	desc.bVsync = false;				//수직동기화 여부
	desc.Handle = NULL;					//윈도우 핸들러 
	desc.Width = WinSizeX;				//윈도우 창 가로길이
	desc.Height = WinSizeY;				//윈도우 창 세로길이
	desc.bToolMode = true;				//툴작업 용으로 로드 여부 
	desc.command = command;				//윈도우 커맨드 값

	Window::SetWindowDesc(desc);		//다 초기화 했으면 세팅해준다 
	srand((UINT)time(0));				//타임시드 초기화 

	Window* window = new Window();		//윈도우 클래스 생성 -> 생성자에서 윈도우 창 생성 및 레지스터 등록한다.
	WPARAM wParam = window->Run();		//이곳에서 게임의 메인 루프가 돈다.
	SafeDelete(window);					//여기까지 왔다는 것은 메인루프가 끝났다는 뜻이므로 윈도우 클래스 삭제 

	return wParam;						//여기까지 왔으면 윈API종료 
}
