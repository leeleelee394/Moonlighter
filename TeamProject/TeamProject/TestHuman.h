#pragma once
#include "GameObject.h"

class TestHuman : public GameObject
{
private:
	bool _isMove;
	int _hp;

public:
	TestHuman(Vector2 pos);
	~TestHuman();

	void Update()override; 
	void Render()override; 

	void MoveOn() { _isMove = true; }
	void Damage(int i)
	{
		this->_hp -= i;
		if (_hp <= 0)
			this->Destroy();
		else
			this->MoveOn();
	}
};

