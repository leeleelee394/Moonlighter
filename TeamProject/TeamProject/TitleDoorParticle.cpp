#include "stdafx.h"
#include "TitleDoorParticle.h"

#include "Animation.h"
TitleDoorParticle::TitleDoorParticle(Vector2 pos,float startTime)
	:_pos(pos),_isPlay(false), _frameCount(0.f), _startTime(startTime)
{
	_image = _ImageManager->FindImage("door_light");
	_animation = new Animation();
	_animation->SetStartEndFrame(0, 0, 18, 0, true);
	_animation->SetIsLoop(true);
	_animation->SetFrameUpdateTime(0.2f);
	_animation->SetCurrentFrameX(Math::Random(0, 18));
	_animation->Play();
}

TitleDoorParticle::~TitleDoorParticle()
{
	SafeDelete(_animation);
}

void TitleDoorParticle::Update()
{
	_animation->UpdateFrame();
}

void TitleDoorParticle::Render()
{
	_image->SetSize(Vector2(5,5));
	_image->FrameRender(CastingInt(_pos.x), CastingInt(_pos.y), _animation->GetNowFrameX(), 0);
}

void TitleDoorParticle::MovePositionX(float moveValue)
{
	_pos.x += moveValue;
}
