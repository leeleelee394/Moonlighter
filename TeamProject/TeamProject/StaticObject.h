#pragma once
#include "GameObject.h"
class StaticObject : public GameObject
{
protected:
	class Image* _image;
	float _angle;
	RECT _collisionRect;
public:
	StaticObject(string imageKey, Vector2 pos,float angle = 0.f);
	~StaticObject();

	void Release()override; 
	void Render()override; 

	void SetCollisionRect(RECT rc) { _collisionRect = rc; }
	virtual RECT GetCollisionRect()const override { return _collisionRect; }
};

