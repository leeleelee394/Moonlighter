#include "stdafx.h"
#include "Rock.h"
#include "Image.h"
#include "Player.h"
Rock::Rock(Vector2 pos, float time)
{
	_name = "Rock";

	_stayTime = time;
	_timeCount = 0.f;

	_firstPosition = _position = pos;
	_AlphaCount = 0.f;
	_size = Vector2(70,70);

	_pivot = Pivot::CENTER;
	this->UpdateMainRect();
	_rockImage = _ImageManager->FindImage("rock");
	_shadowImage = _ImageManager->FindImage("shadow");
	_player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");
}


Rock::~Rock()
{
}


void Rock::Init()
{
}
void Rock::Release()
{
}

void Rock::Update()
{
	UpdateState();
}
void Rock::Render()
{
	if (_state == State::Move)
	{
		_shadowImage->SetSize(_size);
		_shadowImage->SetAlpha(0.4f);
		_shadowImage->Render(_firstPosition.x, _firstPosition.y + 1000, _pivot, true);
	}
	_rockImage->SetSize(_size);
	_rockImage->SetAlpha(1.f - _AlphaCount);
	_rockImage->Render(_position.x, _position.y, _pivot, true);
	if(_isDebug == true)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::blue, true);
	}
}

void Rock::UpdateState()
{
	switch (_state)
	{
	case Rock::State::Idle:
		_timeCount += _TimeManager->DeltaTime();
		if (_stayTime < _timeCount)
		{
			ChangeState(State::Move);
		}
		break;
	case Rock::State::Move:
		_position.y += 1000.f * _TimeManager->DeltaTime();
		this->UpdateMainRect();
		if (_firstPosition.y + 1000 < _position.y)
		{
			_Camera->Shake(3.5f, 0.7f);
			ChangeState(State::Stop);
		}

		break;
	case Rock::State::Stop:
		_timeCount += _TimeManager->DeltaTime();
		this->UpdateMainRect();
		if (_timeCount > 20)
		{
			_timeCount = 0.f;
			ChangeState(State::Delete);
		}
		break;
	case Rock::State::Delete:
		_timeCount += _TimeManager->DeltaTime();
		if (_timeCount > 0.15f)
		{
			_timeCount = 0.f;
			_AlphaCount += 0.1f;
		}
		if (_AlphaCount >= 1.f)
		{
			this->Destroy();
		}
		break;
	case Rock::State::End:
		break;
	default:
		break;
	}
}

void Rock::ChangeState(State state)
{
	if (_state == state)
		return;
	_state = state;
	switch (_state)
	{
	case Rock::State::Idle:
		break;
	case Rock::State::Move:
		break;
	case Rock::State::Stop:
		RECT temp;
		_SoundManager->Play("GolemKingRockHitGround", 0.2f);
		if (IntersectRect(&temp, &_mainRect, &_player->GetCollisionRect()))
		{
			_player->AttackedDamage(30);
		}
		break;
	case Rock::State::Delete:
		break;
	case Rock::State::End:
		break;
	default:
		break;
	}
}

