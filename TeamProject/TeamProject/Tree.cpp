#include "stdafx.h"
#include "Tree.h"


Tree::Tree(Vector2 pos)
	:FrameObject("tree",pos)
{
	_collisionRect = Figure::RectMakeBottom(_position, Vector2(70 , 50));
	_shadowImage = _ImageManager->FindImage("Shadow");
}

Tree::~Tree()
{
}

void Tree::Release()
{
	FrameObject::Release();
}

void Tree::Update()
{
	FrameObject::Update();
}

void Tree::Render()
{
	_shadowImage->SetAlpha(0.4f);
	_shadowImage->SetSize(Vector2(136, 92));
	_shadowImage->Render(_collisionRect.left + 34, _collisionRect.top + 26, Pivot::CENTER, true);

	FrameObject::Render();
}
