#pragma once
#include "GameObject.h"
class TitleBackground : public GameObject
{
private:
	class Animation* _animation;
	class Image* _image;

public:
	TitleBackground();
	~TitleBackground();

	void Release();
	void Update();
	void Render();
};

