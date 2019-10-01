#include "stdafx.h"
#include "Npc.h"
#include "Time.h"
#include "Image.h"
#include "Animation.h"

using namespace Figure;

Npc::Npc()
{
	//_name = "NpcKid";
	movePoint.push_back(Vector2(794,787));
	movePoint.push_back(Vector2(2187,787));
	movePoint.push_back(Vector2(2187,1585));
	movePoint.push_back(Vector2(794,1585));

	_ImageManager->AddFrameImage("Kids", L"../Resources/Npc/Kids.png", 6, 4);
	_imgKids = _ImageManager->FindImage("Kids");

	_ImageManager->AddImage("Shadow", L"../Resources/Object/Shadow.png");
	this->_imgShadow = _ImageManager->FindImage("Shadow");

	this->_isActive = true;
	this->_pivot = Pivot::CENTER;
	_position = movePoint[0];
	_size = Vector2(60, 60);
	
	this->UpdateMainRect();
	_speed = 40.f;
	_index = 1;
	_frameX=0;
	_frameY=3;
	_count = 0;
}

Npc::~Npc()
{
}

void Npc::Init()
{	

}

void Npc::Release()
{

}

void Npc::Update()
{
	float angle;	
	angle = Math::GetAngle(_position.x, _position.y, movePoint[_index].x, movePoint[_index].y);
	  
	_position.x += cos(angle)*_speed*_TimeManager->DeltaTime();
	_position.y -= sin(angle)*_speed*_TimeManager->DeltaTime();

	

	float distance = Math::GetDistance(_position.x, _position.y, movePoint[_index].x, movePoint[_index].y);
	if (distance <= 5.0f)
	{		
		_index++;
		if (movePoint.size() == _index)
		{
			_index = 1;
		}
	}

	_count += _TimeManager->DeltaTime();
	if(_count > 0.3f)
	{
		if (_frameX > 4)
		{
			_frameX = 0;
		}
		else
		{
			_frameX++;
		}		
		_count = 0.0f;
	}
	
	if (_index == 0) _frameY = 1;
	if (_index == 1) _frameY = 3;
	if (_index == 2) _frameY = 0;
	if (_index == 3)_frameY = 2;

	this->UpdateMainRect();
}

void Npc::Render()
{
	//±×¸²ÀÚ
	_imgShadow->SetAlpha(0.4f);
	_imgShadow->SetSize(Vector2(28, 20));
	_imgShadow->Render(this->_position.x, this->_position.y + 24, Pivot::CENTER, true);
	
	_imgKids->SetSize(Vector2(60,60));
	_imgKids->SetScale(1.0f);
	_imgKids->FrameRender(_position.x, _position.y, _frameX, _frameY, Pivot::CENTER, true);

	

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::blue, true);
	}

}

