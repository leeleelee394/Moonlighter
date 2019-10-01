#include "stdafx.h"
#include "Cloud.h"


Cloud::Cloud(Vector2 pos)
{
	_image = _ImageManager->FindImage("Village_Clouds_04");
	_position = pos;
	_size = _image->GetSize();
	_pivot = Pivot::CENTER;
}


Cloud::~Cloud()
{
}

void Cloud::Update()
{
	_position.x -= 10.f * _TimeManager->DeltaTime();
	this->UpdateMainRect();
	if (_mainRect.right < 0)
	{
		_position.x = _Camera->GetMapSize().x + _size.x / 2.f;
		this->UpdateMainRect();
	}
}

void Cloud::Render()
{
	_image->Render(CastingInt(_mainRect.left), CastingInt(_mainRect.top), Pivot::LEFT_TOP, true);
}
