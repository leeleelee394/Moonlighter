#include "stdafx.h"
#include "Npc_Girl.h"
#include "Time.h"
#include "Image.h"
#include "Animation.h"
using namespace Figure;

Npc_Girl::Npc_Girl()
{
	movePoint.push_back(Vector2(2026, 1050));
	movePoint.push_back(Vector2(1000, 1050));
	movePoint.push_back(Vector2(2026, 1050));
	movePoint.push_back(Vector2(1500, 1050));

	_ImageManager->AddFrameImage("Girl", L"../Resources/Npc/Girl.png", 9, 4);
	_imgGirl = _ImageManager->FindImage("Girl");

	_ImageManager->AddImage("Shadow", L"../Resources/Object/Shadow.png");
	this->_imgShadow = _ImageManager->FindImage("Shadow");

	this->_isActive = true;
	this->_pivot = Pivot::CENTER;
	_position = movePoint[0];
	_size = Vector2(90, 78);

	this->UpdateMainRect();
	_speed = 40.f;
	_index = 1;
	_frameX = 0;
	_frameY = 0;
	_count = 0;
}

Npc_Girl::~Npc_Girl()
{
}

void Npc_Girl::Init()
{

}

void Npc_Girl::Release()
{

}

void Npc_Girl::Update()
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
			_index = 0;
		}
	}
	
	_count += _TimeManager->DeltaTime();
	if (_count > 0.2f)
	{
		if (_frameX > 7)
		{
			_frameX = 0;
		}
		else
		{
			_frameX++;
		}
		_count = 0.0f;
	}
	
	if (_index == 0) _frameY = 3;
	if (_index == 1) _frameY = 2;
	if (_index == 2) _frameY = 3;
	if (_index == 3)_frameY = 2;

	this->UpdateMainRect();
	
}

void Npc_Girl::Render()
{
	
	//±×¸²ÀÚ
	_imgShadow->SetAlpha(0.4f);
	_imgShadow->SetSize(Vector2(52, 24));
	_imgShadow->Render(this->_position.x-6, this->_position.y + 44, Pivot::CENTER, true);

	_imgGirl->SetSize(Vector2(92, 92));
	_imgGirl->SetScale(1.0f);
	_imgGirl->FrameRender(_position.x, _position.y, _frameX, _frameY, Pivot::CENTER, true);

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::blue, true);
	}

	//ImGui::Begin("aa");
	//ImGui::Text("%d, %d", _Camera->GetWorldMousePoint().x, _Camera->GetWorldMousePoint().y);
	//ImGui::End();
	
}

