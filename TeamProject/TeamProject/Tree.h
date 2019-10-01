#pragma once
#include "FrameObject.h"
class Tree : public FrameObject
{
private:
	class Image* _shadowImage;
public:
	Tree(Vector2 pos);
	virtual ~Tree();

	void Release()override; 
	void Update()override;
	void Render()override; 

};

