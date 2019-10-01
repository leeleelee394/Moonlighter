#include "stdafx.h"
#include "TitleBackground.h"

#include "Animation.h"

TitleBackground::TitleBackground()
{
	_animation = new Animation;
	_animation->SetStartEndFrame(0, 0, 15, 0, false);
	_animation->SetFrameUpdateTime(0.1f);
	_animation->SetIsLoop(true);
	_animation->Play();

	_image = _ImageManager->AddFrameImage("mainFrame", L"../Resources/Scene/main_frame.png", 16, 1);
}


TitleBackground::~TitleBackground()
{
}

void TitleBackground::Release()
{
	SafeDelete(_animation);
}

void TitleBackground::Update()
{
	_animation->UpdateFrame();
}

void TitleBackground::Render()
{
	_image->SetSize(Vector2(WinSizeX, WinSizeY));
	_image->FrameRender(0, 0, _animation->GetNowFrameX(), _animation->GetNowFrameY(), Pivot::LEFT_TOP);
}
