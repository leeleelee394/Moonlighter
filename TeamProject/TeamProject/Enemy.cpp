#include "stdafx.h"
#include "Image.h"
#include "Enemy.h"
#include "MoveItem.h"
#include "Player.h"
#include "Effect.h"

Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}

void Enemy::Init()
{
	this->_player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");
	_count = 0;
	_attacked = false;
}

//내 뎀지를 넘기기 위한 함수.
void Enemy::AttackedDemege(int damage)
{
	//누나가 데미지를 넘겨주면 데미지만큼 내 체력을 깎는다.
	_hp -= damage;
	//hp가 0보다 작거나 같으면
	if (_hp <= 0)
	{
		//카메라 흔들기
		_Camera->Shake();

		//이팩트 : 폭발
		Effect::PlayEffect(EFFECT_BOOM, _position);

		_SoundManager->Play("enemyDeath", 1.0f);

		//아이템 뿌리기
		this->SprayItemEnemy("item_redCristal");

		//사라져라. 다른 죽는 모션이 존재할 경우 가상함수 상속을 통해서 내용을 바꿀 수도 있다.
		this->Destroy();
	}
	//죽지 않았다면
	else
	{
		//피격을 당했다는 변수를 트루로 만들어 주고
		_attacked = true;

		//카메라 흔들기
		_Camera->Shake();

		//데미지 폰트 출력용
		_DamageFontManager->ShowDamage(_position, damage);

		//뒤로 밀려난다. 플레이어의 앵글을 먼저 넣어주면 기존에 추격하면 방향에서 반대로 앵글값이 나오므로 반대방향으로 밀러날 수 있다.
		this->_attackedAngle = Math::GetAngle(_player->GetPosition().x, _player->GetPosition().y, _position.x, _position.y);
	}
}

//렉트업데이트를 위한 변수
RECT Enemy::UpdateRect(Vector2 position, Vector2 size, Pivot::Enum _pivot)
{
	RECT rc;
	switch (_pivot)
	{
	case Pivot::LEFT_TOP:
		rc = Figure::RectMake(position, size);
		break;
	case Pivot::CENTER:
		rc = Figure::RectMakeCenter(position, size);
		break;
	case Pivot::BOTTOM:
		rc = Figure::RectMakeBottom(position, size);
		break;
	default:
		break;
	}

	return rc;
}

//앵글값에 따라서 무브 타입을 결정.
void Enemy::EnemyMoveType()
{
	//45도 ~ 135도의 방향에서는 탑을 바라보게 한다.
	if (_angle > Math::PI / 4.f && _angle < ((3 * Math::PI) / 4.f))
	{
		_move = MoveType::Top;
	}
	//135도 ~ 225도의 방향에서는 레프트를 바라보게 한다. 
	else if (_angle > ((3.f * Math::PI) / 4.f) && _angle < (Math::PI + (Math::PI / 4.f)))
	{
		_move = MoveType::Left;
	}
	//225도 ~ 315도의 방향에서는 바텀을 바라보게 한다.
	else if (_angle > (Math::PI + (Math::PI / 4.f)) && _angle < (Math::PI + ((3.f * Math::PI) / 4.f)))
	{
		_move = MoveType::Bottom;
	}
	//셋중 무엇도 아니라면 라이트를 바라보게한다.
	else
	{
		_move = MoveType::Right;
	}
}

bool Enemy::IntersectReaction(RECT * moveRect, RECT * unMoveRect)
{

	RECT rc = { 0 };

	//충돌이 안되었다면 빠져나오라
	if (IntersectRect(&rc, moveRect, unMoveRect) == false)
		return false;

	int x = rc.right - rc.left;
	int y = rc.bottom - rc.top;
	if (x > y)
	{
		if (rc.bottom == unMoveRect->bottom)
		{
			moveRect->bottom = moveRect->bottom + y;
			moveRect->top = moveRect->top + y;
		}
		else if (rc.top == unMoveRect->top)
		{
			moveRect->bottom = moveRect->bottom - y;
			moveRect->top = moveRect->top - y;
		}
	}
	else
	{
		if (rc.left == unMoveRect->left)
		{
			moveRect->left = moveRect->left - x;
			moveRect->right = moveRect->right - x;
		}
		else if (rc.right == unMoveRect->right)
		{
			moveRect->left = moveRect->left + x;
			moveRect->right = moveRect->right + x;
		}
	}
	return true;
}


void Enemy::ObjectCollision()
{
	//=======================================
	//오브젝트와 충돌
	//=======================================
	const vector <class GameObject*>* object;
	object = _ObjectManager->GetObjectListPointer(ObjectType::Object);
	for (int i = 0; i < object->size(); i++)
	{

		MoveItem* item = dynamic_cast<MoveItem*>((*object)[i]);
		Player* player = dynamic_cast<Player*>((*object)[i]);

		if (item == nullptr && player == nullptr && this != (*object)[i])
		{
			if (this->IntersectReaction(&_renderRect, &(*object)[i]->GetCollisionRect()))
			{
				_position.x = (_renderRect.right - _renderRect.left) / 2 + _renderRect.left;
				_position.y = (_renderRect.bottom - _renderRect.top) / 2 + _renderRect.top;
				this->_renderRect = UpdateRect(_position, _size, _pivot);
			}
		}
	}
}


//직선거리 길이 구하는 공식.
float Enemy::Distance(Vector2 position)
{
	//직선거리를 구하고
	float distance = Math::GetDistance(position.x, position.y, _player->GetPosition().x, _player->GetPosition().y);
	//반환해
	return distance;
}

//앵글값구하는 공식.
float Enemy::Angle(Vector2 position)
{
	//앵글값을 구하고
	float angle = Math::GetAngle(position.x, position.y, _player->GetPosition().x, _player->GetPosition().y);
	//반환해
	return angle;
}