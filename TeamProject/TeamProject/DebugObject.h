#pragma once
#include "GameObject.h"
class DebugObject : public GameObject
{
	class Image* _image;
public:
	DebugObject(string name);
	~DebugObject();

	void Update()override; 
	void Render()override;  
};

