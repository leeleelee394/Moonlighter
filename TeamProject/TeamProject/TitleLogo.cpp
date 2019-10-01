#include "stdafx.h"
#include "TitleLogo.h"


TitleLogo::TitleLogo()
	:GameObject("TitleLogo", Vector2(WinSizeX / 2, 200), Vector2(0, 0), Pivot::CENTER)
{
	_image = _ImageManager->FindImage("TitleImage");
	_size = Vector2((int)_image->GetWidth(),(int)_image->GetHeight());
	this->UpdateMainRect();
}


TitleLogo::~TitleLogo()
{
}

void TitleLogo::Render()
{
	_image->Render(CastingInt(_mainRect.left), CastingInt(_mainRect.top), Pivot::LEFT_TOP);
}
