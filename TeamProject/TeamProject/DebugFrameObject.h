#pragma once
#include "GameObject.h"
class DebugFrameObject : public GameObject
{
private:
	class Image* _image;
	class Animation* _animation;
	float _frameUpdateTime; 
public:
	DebugFrameObject(string imageKey);
	~DebugFrameObject();

	void Update()override; 
	void Render()override; 
};

