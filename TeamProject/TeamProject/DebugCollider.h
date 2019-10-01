#pragma once
#include "GameObject.h"
class DebugCollider : public GameObject
{
	enum class State
	{
		None,Drag,End
	};
private:
	State _state;
	Vector2 _startPos;
	Vector2 _endPos; 
	RECT _rect; 
public:
	DebugCollider();
	~DebugCollider();

	void Update();
	void Render();
};

