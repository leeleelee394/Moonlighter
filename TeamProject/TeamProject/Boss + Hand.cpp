#include "stdafx.h"
#include "Boss.h"
#include "Image.h"
#include "Animation.h"
#include "Player.h"


void Boss::HandShootHand()
{
	switch (_hand)
	{
		float angle;
		
	case HandState::Up:

		//그림자와 손과의 거리
		_distance = Math::GetDistance(_handPosition.x, _handPosition.y, _shadowPosition.x, _shadowPosition.y);
		
		//거리가 500보다 작으면
		if (_distance <= 700)
		{
			//위로 올라가라.
			_handPosition.y -= (_speed * 0.7) * _TimeManager->DeltaTime();
		}
		_handRc = Figure::RectMakeCenter(_handPosition, _handSize);
		//어느정도 올라갔으면
		if (_distance >= 700)
		{	
			_handPosition.y -= (_speed * 0.0f) * _TimeManager->DeltaTime();
			this->ChangeShadowState(ShadowState::Chasing);
		}
		break;
	case HandState::Down:
		
		//손과 그림자의 앵글값.
		angle = Math::GetAngle(_handPosition.x, _handPosition.y, _shadowPosition.x, _shadowPosition.y);
		//그림자와 손과의 거리
		_distance = Math::GetDistance(_handPosition.x, _handPosition.y, _shadowPosition.x, _shadowPosition.y);

		//그림자와 손의 Y축이 같지 않다면
		if (_distance > 10)
		{
			//그림자의 Y축으로 이동해라.
			_handPosition.y += -sinf(angle) * (_speed * 4) * _TimeManager->DeltaTime();
		}
		_handRc = Figure::RectMakeCenter(_handPosition, _handSize);
		//만약 같다면 
		if (_distance <= 10)
		{
			_Camera->Shake(5.f, 0.5f);
			//잠시간멈춰있어라.
			this->ChangeHandState(HandState::Stay);
		}
		break;
	case HandState::Stay:

		_timeCount += _TimeManager->DeltaTime();
		if (_timeCount > 2.f)
		{
			//손의 상태를 올라가는 상태로 바꿔준다.
			this->ChangeHandState(HandState::Up);
		}
		_handRc = Figure::RectMakeCenter(_handPosition, _handSize);
		break;
	case HandState::Chasing:
		
		//손의 X축은 언제나 그림자의 X축을 따라다닌다.
		angle = Math::GetAngle(_handPosition.x, _handPosition.y, _shadowPosition.x, _shadowPosition.y);
		_handPosition.x = _shadowPosition.x;
		//다섯번떨궜으면 스킬공격을 멈춰라.
		if (_drapCount == 5)
		{
			_drapCount = 0;
			this->ChangeState(StateType::Hand_Shoot_Last);
		}
		_handRc = Figure::RectMakeCenter(_handPosition, _handSize);
		break;
	case HandState::End:
		break;
	default:
		break;
	}
}

void Boss::ChangeHandState(HandState hand)
{
	if (_hand == hand)
		return;

	_hand = hand;

	this->ChangeHandAnimation(hand);

	switch (_hand)
	{
	case HandState::Up:
		//몇번떨어졌는지 알기위해 카운트를 더해주고
		this->_drapCount++;

		break;
	case HandState::Down:
		break;
	case HandState::Stay:
		_SoundManager->Play("GolemKingHandCrush", 0.6f);
		RECT temp;
		if (IntersectRect(&temp, &_handRc, &_player->GetCollisionRect()))
		{
			_player->AttackedDamage(_damage);
		}
		break;
	case HandState::Chasing:
		break;
	case HandState::End:
		break;
	default:
		break;
	}
}

void Boss::ChangeHandAnimation(HandState hand)
{
	_handIter iter = _handList.find(hand);

	if (iter != _handList.end())
	{
		_handAni = iter->second;
		_handAni->Stop();
		_handAni->Play();;
	}
}

void Boss::CreateHandAnimation()
{
	Animation* chasing = new Animation;
	chasing->SetStartEndFrame(0, 0, 0, 0, false);
	chasing->SetIsLoop(false);
	chasing->SetFrameUpdateTime(1.f);
	_handList.insert(make_pair(HandState::Chasing, chasing));

	Animation* up = new Animation;
	up->SetStartEndFrame(16, 0, 18, 0, false);
	up->SetIsLoop(false);
	up->SetFrameUpdateTime(0.1f);
	_handList.insert(make_pair(HandState::Up, up));

	Animation* down = new Animation;
	down->SetStartEndFrame(0, 0, 3, 0, false);
	down->SetIsLoop(false);
	down->SetFrameUpdateTime(0.1f);
	_handList.insert(make_pair(HandState::Down, down));

	Animation* stay = new Animation;
	stay->SetStartEndFrame(3, 0, 16, 0, false);
	stay->SetIsLoop(false);
	stay->SetFrameUpdateTime(0.1f);
	_handList.insert(make_pair(HandState::Stay, stay));
}