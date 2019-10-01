#pragma once
#include "GameObject.h"
class FrameObject : public GameObject
{
protected:
	class Image* _image;
	class Animation* _animation;
	RECT _collisionRect;
public:
	FrameObject(string imageKey,Vector2 pos);
	virtual ~FrameObject();

	void Release()override;
	void Update()override; 
	void Render()override; 

	RECT GetCollisionRect()const override { return this->_collisionRect; }
	void SetCollisionRect(RECT rc) { this->_collisionRect = rc; }
	class Animation* GetAnimation()const { return this->_animation; }
};

