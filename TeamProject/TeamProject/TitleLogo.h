#pragma once
#include "GameObject.h"
class TitleLogo : public GameObject
{
private:
	class Image* _image;
public:
	TitleLogo();
	~TitleLogo();
	
	void Render()override; 
};

