#pragma once
#include "GameObject.h"
class Npc_Girl : public GameObject
{

protected:
	class Image* _imgGirl;
	class Image* _imgShadow;

	float _count;
	RECT _npcRc;
	vector<Vector2>movePoint;
	Vector2 position;

	float _speed;
	int _index;
	int _frameX;
	int _frameY;

	enum class State
	{
		Left, Right, Up, Down, end
	};

	State _state;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

public:
	Npc_Girl();
	~Npc_Girl();
};

