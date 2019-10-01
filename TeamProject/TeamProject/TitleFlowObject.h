#pragma once
#include "GameObject.h"

class TitleFlowObject : public GameObject
{
	enum class State
	{
		Close,OpenSlide,Open 
	};
private:
	State _state;
	class Image* _selectorImage; 
	class TitleSelector* _selectorList[2];
	int _selectIndex;
public:
	TitleFlowObject();
	~TitleFlowObject();

	void Update()override; 
	void Render()override; 

	void DoorOpened();
};

class TitleSelector
{
public:
	wstring name;
	RECT rc;
	function<void()> func;

	TitleSelector(wstring name, RECT rc, function<void()> func)
		:name(name), rc(rc), func(func) {}
};
