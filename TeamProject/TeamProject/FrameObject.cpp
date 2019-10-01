#include "stdafx.h"
#include "FrameObject.h"

#include "Animation.h"

FrameObject::FrameObject(string imageKey,Vector2 pos)
	:GameObject("",pos,Vector2(),Pivot::BOTTOM)
{
	_image = _ImageManager->FindImage(imageKey);
	_size = _image->GetFrameSize(0);
	this->UpdateMainRect();

	_animation = new Animation;
	_animation->SetStartEndFrame(0, 0, _image->GetMaxFrameX() - 1, 0, false);
	_animation->SetFrameUpdateTime(0.3f);
	_animation->SetIsLoop(true);
	_animation->SetCurrentFrameX(Math::Random(0, 3));
	_animation->Play();
}

FrameObject::~FrameObject()
{
}


void FrameObject::Release()
{
	GameObject::Release();
	SafeDelete(_animation);
}

void FrameObject::Update()
{
	GameObject::Update();
	_animation->UpdateFrame();
}

void FrameObject::Render()
{
	_image->SetSize(_size);
	_image->FrameRender(CastingInt(_mainRect.left), CastingInt(_mainRect.top), _animation->GetNowFrameX(),
		_animation->GetNowFrameY(), Pivot::LEFT_TOP, true);

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(_collisionRect, DefaultBrush::red, true);
	}
}
