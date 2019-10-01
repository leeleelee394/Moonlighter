#pragma once
#include "GameObject.h"
class Door :
	public GameObject
{
public:
	static string _mapName;
private:
	RECT rc;
	RECT testCollision;
	Vector2 rcPosition;
	Vector2 rcSize;

	float _timeCount;
	int _doorCount;

	bool _isDoorChange;

	class Image* _door2;
	class Image* _door;
	class Player* _player;
public:
	Door(Vector2 pos, Vector2 size);
	virtual ~Door();

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
};

