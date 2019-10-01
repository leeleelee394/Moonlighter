#include "stdafx.h"
#include "UIPotion.h"


UIPotion::UIPotion()
{
	//포션 이미지 
	_uiPotionIMG = _ImageManager->FindImage("UI_Potion");

	this->_position = Vector2(WinSizeX - 62, -7);
	this->_mainRect = Figure::RectMake(_position, Vector2(50, 50));
}


UIPotion::~UIPotion()
{
}

void UIPotion::Init()
{
}

void UIPotion::Release()
{
}

void UIPotion::Update()
{
}

void UIPotion::Render()
{
	_uiPotionIMG->SetSize(Vector2(70,70));
	_uiPotionIMG->Render(_position.x, _position.y, Pivot::LEFT_TOP, false);

	if (_isDebug) 
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::yello, false, 1.0f);
	}
}
