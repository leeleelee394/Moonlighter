#include "stdafx.h"
#include "Image.h"
#include "Player.h"
#include "MoveItem.h"
#include "Golem.h"
#include "Effect.h"

Golem::Golem(Vector2 pos)
{
	//에너미의 자시자신 기본 설정.
	this->_name = "Golem";					//내이름은 골램이여
	this->_position = pos;					//
	this->_size = Vector2(100, 100);		//크기도 100, 100이야
	this->_pivot = Pivot::CENTER;			//내 기준은 중심에있어
	this->_speed = 90.0f;					//속도는 90.0f
	this->_hp = 100;						//100의 체력
	this->_damage = 12;						//12의 뎀지
	this->_attackSound = false;
	this->_isAttack = false;				//공격은 처음에는 안하고있지
	bool _attackedColor = false;
	this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);

	//내 이미지 찾기!
	this->_golemMove = _ImageManager->FindImage("GolemMove");
	this->_golemMove_Red = _ImageManager->FindImage("GolemMove_Red");
	this->_golemMove_white = _ImageManager->FindImage("GolemMove_White");

	this->_golemAttack = _ImageManager->FindImage("GolemAttack");
	this->_golemAttack_Red = _ImageManager->FindImage("GolemAttack_Red");
	this->_golemAttack_White = _ImageManager->FindImage("GolemAttack_White");

	this->_shadow = _ImageManager->FindImage("Shadow");

	//각종 카운트 초기화
	this->_moveCount = 0;
	this->_attackCount = 0;
	this->_attackedCount = 0;
	this->_countMove = 0.f;
	this->_countAttack = 0.f;

	//공격 렉트를 위한 포지션 초기화
	this->_positionLeft = Vector2(_position.x - 110, _position.y - 10);
	this->_positionRight = Vector2(_position.x + 10, _position.y - 15);
	this->_positionTop = Vector2(_position.x, _position.y - 10);
	this->_positionBottom = Vector2(_position.x, _position.y + 110);

	//공격 렉트를 위한 사이즈 설정 및 초기화
	this->_sizeLeft = Vector2(100, 60);
	this->_sizeRight = Vector2(100, 60);
	this->_sizeTop = Vector2(60, 70);
	this->_sizeBottom = Vector2(60, 80);

	//공격여부확인을 위한 불변수 초기화
	this->_isAttackTop = false;
	this->_isAttackLeft = false;
	this->_isAttackRight = false;
	this->_isAttackBottom = false;

	//인벤토리가 열리고 닫힐때 움직임에 제한을 주기 위해서 넣어놓은 콜백메세지
	this->AddCallbackMessage("InventoryOpen", [this](TagMessage message) {this->InvenStop(1); });
	this->AddCallbackMessage("InventoryClose", [this](TagMessage message) {this->InvenStop(0); });
}

Golem::~Golem()
{
}

void Golem::Init()
{
	Enemy::Init();
}

void Golem::Release()
{
	//
}

void Golem::Update()
{
	_countMove +=_TimeManager->DeltaTime();
	if (_countMove >= 1.f /6.0f)
	{
		_countMove = 0;
		_moveCount++;
	}

	if (!_isStop)
	{
		this->Move();
		this->Attack();
	}
	this->EnemyMoveType();
	this->ImageCount();
	this->AttackPosition();
	this->Collision();
	this->ObjectCollision();

	if (_Input->GetKeyDown('0'))
	{
		this->AttackedDemege(0);
	}
}

void Golem::Render()
{
	this->RectRender();
	this->ImageRender();
}

//프레임 돌리기 위한 함수.
void Golem::ImageCount()
{
	//이미지 프레임돌리기(쫒는상태)
	if (_state == StateType::Chasing)
	{
		if (_moveCount >= 8)
			_moveCount = 0;
	}

	//이미지 프레임 돌리기 (공격상태)
	if (_state == StateType::attack)
	{
		if (_attackCount == 9)
		{
			if (_attackSound == false)
			{
				_SoundManager->Play("golem_dungeon_golem_crash", 0.3);
				_attackSound = true;
			}
		}
		if (_attackCount > 14)
			_attackCount = 0;
	}
}

//공격 사거리를 계산하기 위해서 직선거리계산 및 판정 범위 안일경우 상태 변경
void Golem::Attack()
{
	_distance = Math::GetDistance(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);

	if (_distance < (_size.x * 1.2f))
	{
		_state = StateType::attack;
	}
	if (_state == StateType::attack)
	{
		_countAttack +=_TimeManager->DeltaTime();
		if (_countAttack >= 1.f / 8.0f)
		{
			_countAttack = 0;
			_attackCount++;
		}
	}
	if (_attackCount > 12)
	{
		_attackCount = 0;
		_state = StateType::Chasing;
		_attackSound = false;
	}
}

//각기 공격렉트들의 방향을 조정해주고 공격했을시에 새로 업데이트 해주는 부분.
void Golem::AttackPosition()
{
	this->_positionLeft = Vector2(_position.x - 110, _position.y - 10);
	this->_positionRight = Vector2(_position.x + 10, _position.y - 15);
	this->_positionTop = Vector2(_position.x, _position.y - 10);
	this->_positionBottom = Vector2(_position.x, _position.y + 110);

	if (_state == StateType::attack)
	{
  		if (_move == MoveType::Left && _attackCount > 9)
		{
			this->_isAttackLeft = true;
			this->_attackLeft = UpdateRect(_positionLeft, _sizeLeft, Pivot::LEFT_TOP);
		}
		else _isAttackLeft = false;

		if (_move == MoveType::Right && _attackCount > 9)
		{
			this->_isAttackRight = true;
			this->_attackRight = UpdateRect(_positionRight, _sizeRight, Pivot::LEFT_TOP);
		}
		else _isAttackRight = false;
		if (_move == MoveType::Top && _attackCount > 9)
		{
			this->_isAttackTop = true;
			this->_attackTop = UpdateRect(_positionTop, _sizeTop, Pivot::BOTTOM);
		}
		else _isAttackTop = false;
		if (_move == MoveType::Bottom && _attackCount > 9)
		{
			this->_isAttackBottom = true;
			this->_attackBottom = UpdateRect(_positionBottom, _sizeBottom, Pivot::BOTTOM);
		}
		else _isAttackBottom = false;
	}
}

//공격렉트들의 렌더링에 관한 부분.
void Golem::AttackRender()
{
	if (_state == StateType::attack)
	{
		if (this->_isAttackLeft == true)
		{
			_DXRenderer->DrawRectangle(_attackLeft, DefaultBrush::yello, true);
		}
		if (this->_isAttackRight == true)
		{
			_DXRenderer->DrawRectangle(_attackRight, DefaultBrush::yello, true);
		}
		if (this->_isAttackTop == true)
		{
			_DXRenderer->DrawRectangle(_attackTop, DefaultBrush::yello, true);
		}
		if (this->_isAttackBottom == true)
		{
			_DXRenderer->DrawRectangle(_attackBottom, DefaultBrush::yello, true);
		}
	}
}

//플레이어와 내 공격렉트나 또는 내 골램의 몸과 충돌처리에 관한 부분.
void Golem::Collision()
{
	RECT CollisionRc = { 0 };
	if (this->_isAttackLeft == true)
	{
		if (IntersectRect(&CollisionRc, &_attackLeft, &_player->GetCollisionRect()))
		{
			_isAttack = true;
		}
	}
	if (this->_isAttackRight == true)
	{
		if (IntersectRect(&CollisionRc, &_attackRight, &_player->GetCollisionRect()))
		{
			_isAttack = true;
		}
	}
	if (this->_isAttackTop == true)
	{
		if (IntersectRect(&CollisionRc, &_attackTop, &_player->GetCollisionRect()))
		{
			_isAttack = true;
		}
	}
	if (this->_isAttackBottom == true)
	{
		if (IntersectRect(&CollisionRc, &_attackBottom, &_player->GetCollisionRect()))
		{
			_isAttack = true;
		}
	}
	if (IntersectRect(&CollisionRc, &_renderRect, &_player->GetCollisionRect()))
	{
		_isAttack = true;
	}

	//공격하고 있는 상황이고
	if (_state == StateType::attack)
	{
		//그리고 공격에 성공했으면
		if (_isAttack == true)
		{
			//내 뎀지를 넘겨줘라.
			_player->AttackedDamage(_damage);
			//공격을 완료 했으면 데미지를 주는 변수를 펄스시켜라.
			_isAttack = false;

			_isAttackLeft = false;
			_isAttackRight = false;
			_isAttackTop = false;
			_isAttackBottom = false;
		}
	}
}

//디버깅을 위해서 렉트를 보여주기 위해서 만든 렉트 렌더링용 함수.
void Golem::RectRender()
{
	if (_isDebug == true)
	{
		_DXRenderer->DrawRectangle(_renderRect, DefaultBrush::gray, true);
		if (_state == StateType::Chasing)
		{
			_DXRenderer->DrawEllipse(_position, (_size.x * 1.2f), DefaultBrush::blue, true);
		}
		if (_state == StateType::attack)
		{
			_DXRenderer->DrawEllipse(_position, (_size.x * 1.2f), DefaultBrush::red, true);
		}
		this->AttackRender();
	}
}

//이미지를 렌더링 해주는 함수.
void Golem::ImageRender()
{
	_shadow->SetSize(_shadow->GetFrameSize(0));
	_shadow->SetAlpha(0.3f);
	_shadow->SetScale(0.7f);
	_shadow->Render(_position.x, _position.y + 50, Pivot::CENTER, true);

	if (_state == StateType::Chasing)
	{
		_golemMove->SetSize(_golemMove->GetFrameSize(0));
		_golemMove->SetScale(1.0f);

		_golemMove_Red->SetSize(_golemMove_Red->GetFrameSize(0));
		_golemMove_Red->SetScale(1.0f);
		_golemMove_white->SetSize(_golemMove_white->GetFrameSize(0));
		_golemMove_white->SetScale(1.0f);

		if (_move == MoveType::Left)
		{
			if (!_attacked)
			{
				_golemMove->FrameRender(_position.x, _position.y, _moveCount, 0, Pivot::CENTER, true);
			}
			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemMove_Red->FrameRender(_position.x, _position.y, _moveCount, 0, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemMove_white->FrameRender(_position.x, _position.y, _moveCount, 0, Pivot::CENTER, true);
				}
			}
		}
		if (_move == MoveType::Right)
		{
			if (!_attacked)
			{
				_golemMove->FrameRender(_position.x, _position.y, _moveCount, 1, Pivot::CENTER, true);
			}
			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemMove_Red->FrameRender(_position.x, _position.y, _moveCount, 1, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemMove_white->FrameRender(_position.x, _position.y, _moveCount, 1, Pivot::CENTER, true);
				}
			}
		}
		if (_move == MoveType::Top)
		{
			if (!_attacked)
			{
				_golemMove->FrameRender(_position.x, _position.y, _moveCount, 2, Pivot::CENTER, true);
			}
			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemMove_Red->FrameRender(_position.x, _position.y, _moveCount, 2, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemMove_white->FrameRender(_position.x, _position.y, _moveCount, 2, Pivot::CENTER, true);
				}
			}
		}
		if (_move == MoveType::Bottom)
		{
			if (!_attacked)
			{
				_golemMove->FrameRender(_position.x, _position.y, _moveCount, 3, Pivot::CENTER, true);
			}
			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemMove_Red->FrameRender(_position.x, _position.y, _moveCount, 3, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemMove_white->FrameRender(_position.x, _position.y, _moveCount, 3, Pivot::CENTER, true);
				}
			}
		}
	}

	if (_state == StateType::attack)
	{
		_golemAttack->SetSize(_golemAttack->GetFrameSize(0));
		_golemAttack->SetScale(1.0f);

		_golemAttack_Red->SetSize(_golemAttack_Red->GetFrameSize(0));
		_golemAttack_Red->SetScale(1.0f);
		_golemAttack_White->SetSize(_golemAttack_White->GetFrameSize(0));
		_golemAttack_White->SetScale(1.0f);

		if (_move == MoveType::Left)
		{
			if (!_attacked)
			{
				_golemAttack->FrameRender(_position.x, _position.y, _attackCount, 0, Pivot::CENTER, true);
			}

			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemAttack_Red->FrameRender(_position.x, _position.y, _moveCount, 0, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemAttack_White->FrameRender(_position.x, _position.y, _moveCount, 0, Pivot::CENTER, true);
				}
			}
		}
		if (_move == MoveType::Right)
		{
			if (!_attacked)
			{
				_golemAttack->FrameRender(_position.x, _position.y, _attackCount, 1, Pivot::CENTER, true);
			}

			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemAttack_Red->FrameRender(_position.x, _position.y, _moveCount, 1, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemAttack_White->FrameRender(_position.x, _position.y, _moveCount, 1, Pivot::CENTER, true);
				}
			}
		}
		if (_move == MoveType::Top)
		{
			if (!_attacked)
			{
				_golemAttack->FrameRender(_position.x, _position.y, _attackCount, 2, Pivot::CENTER, true);
			}

			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemAttack_Red->FrameRender(_position.x, _position.y, _moveCount, 2, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemAttack_White->FrameRender(_position.x, _position.y, _moveCount, 2, Pivot::CENTER, true);
				}
			}
		}
		if (_move == MoveType::Bottom)
		{
			if (!_attacked)
			{
				_golemAttack->FrameRender(_position.x, _position.y, _attackCount, 3, Pivot::CENTER, true);
			}

			if (_attacked)
			{
				if (_attackedColor == false)
				{
					_golemAttack_Red->FrameRender(_position.x, _position.y, _moveCount, 3, Pivot::CENTER, true);
				}
				if (_attackedColor == true)
				{
					_golemAttack_White->FrameRender(_position.x, _position.y, _moveCount, 3, Pivot::CENTER, true);
				}
			}
		}
	}
}

//인벤토리의 온오프시 움직임에 제한을 주려고 만든 함수.
void Golem::InvenStop(bool stop)
{
	_isStop = stop;
}

void Golem::AttackedDemege(int damage)
{
	_SoundManager->Play("golemHit", 1.0f);
	//누나가 데미지를 넘겨주면 데미지만큼 내 체력을 깎는다.
	_hp -= damage;
	//hp가 0보다 작거나 같으면
	if (_hp <= 0)
	{
		//아이템 뿌리기
		this->SprayItemEnemy("item_golemCore");

		//카메라 흔들기
		_Camera->Shake();

		//이팩트 : 폭발
		Effect::PlayEffect(EFFECT_BOOM, _position);

		_SoundManager->Play("enemyDeath", 1.0f);
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

//쫒거나 피격당했을 시 움직이기 위한 함수.
//아파요 싫어요 하지마세요
void Golem::Move()
{
	//쫒을대상 추격을 위한 앵글값계산과 이동을 위한 변수들
	if (_state == StateType::Chasing)
	{
		//쫒기위한 앵글값.
		this->_angle = Math::GetAngle(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);
		this->_position.x += cosf(_angle) * _speed * _TimeManager->DeltaTime();
		this->_position.y += -sinf(_angle)*_speed * _TimeManager->DeltaTime();
		this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);
	}

	//공격을 맞았을때 출력해줄 이미지
	if (_attacked)
	{
		_count += _TimeManager->DeltaTime();
		if (_count <= 0.5f)
		{
			this->_position.x += cosf(_attackedAngle) * _speed * _TimeManager->DeltaTime()* 1.2f;
			this->_position.y += -sinf(_attackedAngle) * _speed * _TimeManager->DeltaTime()* 1.2f;
			this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);

			if (_count < 0.2f)
			{
				_attackedColor = false;
			}
			if (_count > 0.2f)
			{
				_attackedColor = true;
			}
		}
		if (_count > 0.5f)
		{
			_attacked = false;
			_count = 0.f;
		}
	}
}

