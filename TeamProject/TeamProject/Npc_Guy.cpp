#include "stdafx.h"
#include "Npc_Guy.h"
#include "Time.h"
#include "Image.h"
#include "Animation.h"
using namespace Figure;

Npc_Guy::Npc_Guy()
{
	movePoint.push_back(Vector2(1608, 1282));
	movePoint.push_back(Vector2(1440, 1282));
	movePoint.push_back(Vector2(1440, 2108));
	movePoint.push_back(Vector2(2326, 2108));
	movePoint.push_back(Vector2(2326, 1651));

	_ImageManager->AddFrameImage("Guy", L"../Resources/Npc/Guy.png", 9, 4);
	_imgGuy = _ImageManager->FindImage("Guy");

	_ImageManager->AddImage("Shadow", L"../Resources/Object/Shadow.png");
	this->_imgShadow = _ImageManager->FindImage("Shadow");

	this->_isActive = true;
	this->_pivot = Pivot::CENTER;
	this->_position = movePoint[0];
	this->_size = Vector2(90, 78);
	
	this->_speed = 40.f;
	this->_index = 1;
	this->_frameX = 0;
	this->_frameY = 0;

	this->_count = 0.0f;
	this->_pauseTime = 0.0f;
	this->_isArrive = false;
	this->UpdateMainRect();
	
}
Npc_Guy::~Npc_Guy()
{}

void Npc_Guy::Init()
{}

void Npc_Guy::Release()
{}

void Npc_Guy::Update()
{	
	//잠시 자리에 머물기 위한 조건과 타이머
	if (_isArrive == true)
	{
		_pauseTime += _TimeManager->DeltaTime();

		if (_pauseTime > 2.0f)
		{
			_isArrive = false;
			_pauseTime = 0.0f;
			_index++;
			if (movePoint.size() <= _index)
			{
				_index = 0;
			}

		}
	}
	else
	{
		_position.x += cos(_angle)*_speed*_TimeManager->DeltaTime();
		_position.y -= sin(_angle)*_speed*_TimeManager->DeltaTime();
	}

	if (_distance <= 5.0f)
	{
		_isArrive = true;
	}
	//거리값을 구해서 다음 위치값으로 이동을 했을때
	//가까워지면 다음 위치값으로 값을 변경한다.
	_distance = Math::GetDistance(_position.x, _position.y, movePoint[_index].x, movePoint[_index].y);

	this->_angle = Math::GetAngle(_position.x, _position.y, movePoint[_index].x, movePoint[_index].y);
	this->MoveType();
	this->StateType();


	//위치값에 맞춰 프레임을 변경한다.
	//프레임을 돌리는 시간을 준다.
	_count += _TimeManager->DeltaTime();
	if (_count > 0.2f)
	{
		//머무르지 않을때만 동작한다
		if (_isArrive == false)
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

	}

	this->UpdateMainRect();
}

void Npc_Guy::Render()
{
	//그림자
	_imgShadow->SetAlpha(0.4f);
	_imgShadow->SetSize(Vector2(64, 24));
	_imgShadow->Render(this->_position.x, this->_position.y + 38, Pivot::CENTER, true);

	_imgGuy->SetSize(Vector2(92, 92));
	_imgGuy->SetScale(1.0f);
	_imgGuy->FrameRender(_position.x, _position.y, _frameX, _frameY, Pivot::CENTER, true);

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::blue, true);
	}

	
}

//앵글값에 따라서 무브 타입을 결정.
void Npc_Guy::MoveType()
{
	//45도 ~ 135도의 방향에서는 탑을 바라보게 한다.
	if (_angle > Math::PI / 4.f && _angle < ((3 * Math::PI) / 4.f))
	{
		_state = State::Up;
	}
	//135도 ~ 225도의 방향에서는 레프트를 바라보게 한다. 
	else if (_angle > ((3.f * Math::PI) / 4.f) && _angle < (Math::PI + (Math::PI / 4.f)))
	{
		_state = State::Left;
	}
	//225도 ~ 315도의 방향에서는 바텀을 바라보게 한다.
	else if (_angle > (Math::PI + (Math::PI / 4.f)) && _angle < (Math::PI + ((3.f * Math::PI) / 4.f)))
	{
		_state = State::Down;
	}
	//셋중 무엇도 아니라면 라이트를 바라보게한다.
	else
	{
		_state = State::Right;
	}
}

void Npc_Guy::StateType()
{
	switch (_state)
	{
	case State::Left:
		_frameY = 2;
		break;
	case State::Right:
		_frameY = 3;
		break;
	case State::Up:
		_frameY = 1;
		break;
	case State::Down:
		_frameY = 0;
		break;
	}
}