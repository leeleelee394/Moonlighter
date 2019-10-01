#pragma once
#include "GameObject.h"
class Rock : public GameObject
{
	enum class State
	{
		Idle, Move, Stop, Delete, End
	};
private:
	class Player* _player;
	class Image* _rockImage;
	class Image* _shadowImage;
	float _stayTime;
	float _timeCount;
	float _AlphaCount;
	State _state;
	Vector2 _firstPosition;
public:

	void ChangeState(State state);
	void UpdateState();

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
public:
	Rock(Vector2 pos, float time);
	~Rock();
};

