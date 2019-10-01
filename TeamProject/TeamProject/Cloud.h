#pragma once
#include "GameObject.h"
class Cloud : public GameObject
{
	class Image* _image;
public:
	Cloud(Vector2 pos);
	~Cloud();
	
	void Update()override; 
	void Render()override; 
};

