#pragma once
#include "StaticObject.h"
class OilLamp : public StaticObject
{
	RECT _collisionRect;
public:
	OilLamp(string imageKey,Vector2 pos);
	~OilLamp();

	void Update()override; 
	void Render()override; 

	RECT GetCollisionRect()const override { return _collisionRect; }
};

