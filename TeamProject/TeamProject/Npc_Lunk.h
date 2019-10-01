#pragma once
#include "GameObject.h"
class Npc_Lunk : public GameObject
{
	class Image* _imgLunk;
	class Image* _imgShadow;

	RECT _npcRc;
	vector<Vector2>movePoint;

	float _speed;
	int _index;
	int _frameX;
	int _frameY;

	float _count;
	float _pauseTime;
	bool _isArrive;
	float _angle;
	float _distance;
	enum class State
	{
		Left, Right, Up, Down, end
	};
	State _state;

public:
	Npc_Lunk();
	~Npc_Lunk();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void MoveType();
	void StateType();
};

