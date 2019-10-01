#include "stdafx.h"
#include "Input.h"

SingletonCpp(Input)

/*********************************************************************
## Input ##
**********************************************************************/
Input::Input()
{
	//처음엔 모든 키를 눌려있지 않은 상태로 초기화
	keyCurrent.reset();
	keyPast.reset();

	mousePosition.x = mousePosition.y = 0;
}
/*********************************************************************
## ~Input ## 
**********************************************************************/
Input::~Input() {}

/*********************************************************************
## Update ## 
과거키를 현재키로 갱신 
**********************************************************************/
void Input::Update()    
{
	keyPast = keyCurrent;	
}
/*********************************************************************
## GetKeyDown ## 
한번 눌렸을 때 
@@ int key : 키 

@@ return bool : keyDown여부 
**********************************************************************/
bool Input::GetKeyDown(int key)
{
	//해당 키를 누른 상태
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//그 키가 눌려있지 않았다면
		if (!keyPast[key])
		{
			//현재키를 눌린 상태로 바꾸고 return true
			keyCurrent.set(key, true);
			return true;
		}
	}
	//해당 키를 누르지 않은 상태
	else keyCurrent.set(key, false);   //현재키를 누르지 않은 상태로 바꿈

	return false;
}

/*********************************************************************
## GetKeyUp ## 
누른 키에서 땠을 때
@@ int key : 키

@@ return bool : keyUp여부 
**********************************************************************/
bool Input::GetKeyUp(int key)
{
	//해당 키를 누른 상태					현재키를 눌린 상태로 바꿈
	if (GetAsyncKeyState(key) & 0x8000) keyCurrent.set(key, true);
	//해당 키를 누르지 않은 상태
	else
	{
		//그 키가 눌려있었다면
		if (keyPast[key])
		{
			//현재키를 누르지 않은 상태로 바꾸고 return true
			keyCurrent.set(key, false);
			return true;
		}
	}

	return false;
}

/*********************************************************************
## Getkey ##
해당 키가 눌려 있는지 
@@ int key : 키 

@@ return bool : StayKey여부 
**********************************************************************/
bool Input::GetKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;

	return false;
}

/*********************************************************************
## IsToggleKey ## 
@@ int key : 키

@@ return bool : 토글여부 
**********************************************************************/
bool Input::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;

	return false;
}
/*********************************************************************
## InputProc ## 
@@ UINT message : 프록시 메세지 
@@ WPARAM wparam : 키 입력 관련 메세지
@@ LPARAM lParam : 마우스 관련 메세지 
**********************************************************************/
LRESULT Input::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_MOUSEMOVE)
	{
		mousePosition.x = static_cast<float>LOWORD(lParam);
		mousePosition.y = static_cast<float>HIWORD(lParam);
	}

	return TRUE;
}

/*********************************************************************
## GetMousePositionPoint ## 
마우스 좌표 PONT 형으로 반환

@@ return POINT : 마우스좌표 포인트 반환
**********************************************************************/

 POINT Input::GetMousePositionPoint()
{
	POINT p;
	p.x = (LONG)mousePosition.x;
	p.y = (LONG)mousePosition.y;
	return p;
}
