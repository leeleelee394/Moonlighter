#include "stdafx.h"
#include "OilLamp.h"

OilLamp::OilLamp(string imageKey, Vector2 pos)
	:StaticObject(imageKey, pos)
{
	this->SetPivot(Pivot::BOTTOM);
	this->_mainRect = Figure::RectMakeBottom(_position, _image->GetSize());
	_collisionRect = Figure::RectMakeBottom(_position, Vector2(50, 50));
}

OilLamp::~OilLamp()
{
}

void OilLamp::Update()
{
}

void OilLamp::Render()
{
	_image->Render((int)_mainRect.left, (int)_mainRect.top, Pivot::LEFT_TOP, true);

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(_collisionRect, DefaultBrush::red, true);
	}
}
