#pragma once
#include "GameObject.h"

class TestDeadman : public GameObject
{
	bool _isMove;
public:
	TestDeadman(Vector2 pos,Vector2 size);
	virtual ~TestDeadman();

	void Init()override; 
	void Update()override; 
	void Render()override; 
	
	void MoveOff() {
		_isMove = false;
	}
};

