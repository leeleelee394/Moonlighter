#pragma once
#include <bitset>

#define KEYMAX 256

class Input
{
	SingletonHeader(Input)
private:
	std::bitset<KEYMAX> keyPast;
	std::bitset<KEYMAX> keyCurrent;
	Vector2 mousePosition;
public:
	void Update();

	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
	bool GetKey(int key);
	bool IsToggleKey(int key);


	inline Vector2 GetMousePosition()const { return this->mousePosition; }
	POINT GetMousePositionPoint();
public:
	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#define _Input Input::Get()
