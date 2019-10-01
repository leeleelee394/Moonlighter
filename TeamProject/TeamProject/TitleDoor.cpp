#include "stdafx.h"
#include "TitleDoor.h"

#include "Image.h"
#include "TitleDoorParticle.h"
TitleDoor::TitleDoor(string key, Vector2 direction,Vector2 pos,Vector2 size)
	:GameObject("TitleDoor",pos,size,Pivot::LEFT_TOP),_direction(direction)
{
	_state = State::Close;
	_image = _ImageManager->FindImage(key);
	_delayTime = _moveRange = 0.f;
	_speed = 500.0f;

	for (UINT i = 0; i < 30; ++i)
	{
		Vector2 randomPos;
		if (pos.x < 100.f)
			randomPos.x = Math::Random(CastingFloat(_mainRect.right - 300.f), CastingFloat(_mainRect.right));
		else
			randomPos.x = Math::Random(CastingFloat(_mainRect.left), CastingFloat(_mainRect.left + 300.f));

		randomPos.y = Math::Random(0.f, CastingFloat(WinSizeY));
		_particles.push_back(new TitleDoorParticle(randomPos, Math::Random(0.f, 3.f)));
	}

	this->AddCallbackMessage("Open", [this](TagMessage message)
	{
		this->StateOpen();
	});

}

TitleDoor::~TitleDoor()
{
	for (UINT i = 0; i < _particles.size(); ++i)
	{
		SafeDelete(_particles[i]);
	}
}

void TitleDoor::Update()
{
	GameObject::Update();

	float moveValue = 0.f;
	switch (_state)
	{
	case TitleDoor::State::Close:
		break;
	case TitleDoor::State::OpenSlide0:
		moveValue = _direction.x * _speed * _TimeManager->DeltaTime();
		_position.x += moveValue;
		_moveRange += Math::Abs(_direction.x * _speed * _TimeManager->DeltaTime());
		this->UpdateMainRect();

		for (UINT i = 0; i < _particles.size(); ++i)
			_particles[i]->MovePositionX(moveValue);

		if (_moveRange >= 100.0f)
		{
			_state = State::OpenSlide1;
			_speed = 550.0f; 
			_moveRange = 0.f;
			_position.y = 0.f;
			this->UpdateMainRect();
		}
		break;
	case TitleDoor::State::OpenSlide1:
		_delayTime += _TimeManager->DeltaTime();
		if (_delayTime >= 0.2f)
		{
			_state = State::OpenSlide2;
		}
		break;
	case TitleDoor::State::OpenSlide2:
		moveValue = _direction.x * _speed * _TimeManager->DeltaTime();
		_position.x += moveValue;
		_moveRange += Math::Abs(_direction.x * _speed * _TimeManager->DeltaTime());
		this->UpdateMainRect();

		for (UINT i = 0; i < _particles.size(); ++i)
			_particles[i]->MovePositionX(moveValue);

		if (_moveRange >= 300.0f)
		{
			_state = State::Open;
			GameObject* flowObject = _ObjectManager->FindObject(ObjectType::Object, "TitleFlowObject");
			if (flowObject)
				flowObject->SendCallbackMessage(TagMessage("DoorOpened",1.0f));
			_position.y = 0.f;
			this->UpdateMainRect();
		}
		break;
	case TitleDoor::State::Open:
		break;
	default:
		break;
	}

	for (UINT i = 0; i < _particles.size(); ++i)
	{
		_particles[i]->Update();
	}
}

void TitleDoor::Render()
{
	_image->SetSize(_size);
	_image->Render(CastingInt(_mainRect.left), CastingInt(_mainRect.top), Pivot::LEFT_TOP, false);
	for (UINT i = 0; i < _particles.size(); ++i)
	{
		_particles[i]->Render();
	}

}
