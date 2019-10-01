#pragma once
/*************************************************************
## WindowDesc ##
윈도우 생성 관련된 정보 담을 구조체 
윈메인.cpp에서 초기화 해준다. 
*************************************************************/
struct WindowDesc
{
	wstring AppName;
	HINSTANCE Instance;
	HWND Handle;
	HDC Hdc;
	float Width;
	float Height;
	int command;
	bool bVsync;
	bool bFullScreen;
	bool bToolMode;
};
/*************************************************************
## Window ##
윈도우창 생성 및 레지스터 등록, 게임의 루프 및 메세지 프록시 관리 하는 클래스 
*************************************************************/
class Window
{
private:
	static class Program* program;		//MainGame이랑 같은 역할 
	static WindowDesc windowDesc;		//윈도우 정보 담을 구조체 
public:
	static WindowDesc GetWindowDesc() { return windowDesc; }
	static void SetWindowDesc(WindowDesc desc) { windowDesc = desc; }
	static bool GetIsToolMode() { return windowDesc.bToolMode; }
public:
	Window();
	~Window();

	WPARAM Run();
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void SetRegister();
	void CreateWindowApi();
	void CreateManager();
	void DeleteManager();
	void UpdateManager();
};

