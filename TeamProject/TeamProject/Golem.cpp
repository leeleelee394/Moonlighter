#include "stdafx.h"
#include "Image.h"
#include "Player.h"
#include "MoveItem.h"
#include "Golem.h"
#include "Effect.h"

Golem::Golem(Vector2 pos)
{
	//���ʹ��� �ڽ��ڽ� �⺻ ����.
	this->_name = "Golem";					//���̸��� ���̿�
	this->_position = pos;					//
	this->_size = Vector2(100, 100);		//ũ�⵵ 100, 100�̾�
	this->_pivot = Pivot::CENTER;			//�� ������ �߽ɿ��־�
	this->_speed = 90.0f;					//�ӵ��� 90.0f
	this->_hp = 100;						//100�� ü��
	this->_damage = 12;						//12�� ����
	this->_attackSound = false;
	this->_isAttack = false;				//������ ó������ ���ϰ�����
	bool _attackedColor = false;
	this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);

	//�� �̹��� ã��!
	this->_golemMove = _ImageManager->FindImage("GolemMove");
	this->_golemMove_Red = _ImageManager->FindImage("GolemMove_Red");
	this->_golemMove_white = _ImageManager->FindImage("GolemMove_White");

	this->_golemAttack = _ImageManager->FindImage("GolemAttack");
	this->_golemAttack_Red = _ImageManager->FindImage("GolemAttack_Red");
	this->_golemAttack_White = _ImageManager->FindImage("GolemAttack_White");

	this->_shadow = _ImageManager->FindImage("Shadow");

	//���� ī��Ʈ �ʱ�ȭ
	this->_moveCount = 0;
	this->_attackCount = 0;
	this->_attackedCount = 0;
	this->_countMove = 0.f;
	this->_countAttack = 0.f;

	//���� ��Ʈ�� ���� ������ �ʱ�ȭ
	this->_positionLeft = Vector2(_position.x - 110, _position.y - 10);
	this->_positionRight = Vector2(_position.x + 10, _position.y - 15);
	this->_positionTop = Vector2(_position.x, _position.y - 10);
	this->_positionBottom = Vector2(_position.x, _position.y + 110);

	//���� ��Ʈ�� ���� ������ ���� �� �ʱ�ȭ
	this->_sizeLeft = Vector2(100, 60);
	this->_sizeRight = Vector2(100, 60);
	this->_sizeTop = Vector2(60, 70);
	this->_sizeBottom = Vector2(60, 80);

	//���ݿ���Ȯ���� ���� �Һ��� �ʱ�ȭ
	this->_isAttackTop = false;
	this->_isAttackLeft = false;
	this->_isAttackRight = false;
	this->_isAttackBottom = false;

	//�κ��丮�� ������ ������ �����ӿ� ������ �ֱ� ���ؼ� �־���� �ݹ�޼���
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

//������ ������ ���� �Լ�.
void Golem::ImageCount()
{
	//�̹��� �����ӵ�����(�i�»���)
	if (_state == StateType::Chasing)
	{
		if (_moveCount >= 8)
			_moveCount = 0;
	}

	//�̹��� ������ ������ (���ݻ���)
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

//���� ��Ÿ��� ����ϱ� ���ؼ� �����Ÿ���� �� ���� ���� ���ϰ�� ���� ����
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

//���� ���ݷ�Ʈ���� ������ �������ְ� ���������ÿ� ���� ������Ʈ ���ִ� �κ�.
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

//���ݷ�Ʈ���� �������� ���� �κ�.
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

//�÷��̾�� �� ���ݷ�Ʈ�� �Ǵ� �� ���� ���� �浹ó���� ���� �κ�.
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

	//�����ϰ� �ִ� ��Ȳ�̰�
	if (_state == StateType::attack)
	{
		//�׸��� ���ݿ� ����������
		if (_isAttack == true)
		{
			//�� ������ �Ѱ����.
			_player->AttackedDamage(_damage);
			//������ �Ϸ� ������ �������� �ִ� ������ �޽����Ѷ�.
			_isAttack = false;

			_isAttackLeft = false;
			_isAttackRight = false;
			_isAttackTop = false;
			_isAttackBottom = false;
		}
	}
}

//������� ���ؼ� ��Ʈ�� �����ֱ� ���ؼ� ���� ��Ʈ �������� �Լ�.
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

//�̹����� ������ ���ִ� �Լ�.
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

//�κ��丮�� �¿����� �����ӿ� ������ �ַ��� ���� �Լ�.
void Golem::InvenStop(bool stop)
{
	_isStop = stop;
}

void Golem::AttackedDemege(int damage)
{
	_SoundManager->Play("golemHit", 1.0f);
	//������ �������� �Ѱ��ָ� ��������ŭ �� ü���� ��´�.
	_hp -= damage;
	//hp�� 0���� �۰ų� ������
	if (_hp <= 0)
	{
		//������ �Ѹ���
		this->SprayItemEnemy("item_golemCore");

		//ī�޶� ����
		_Camera->Shake();

		//����Ʈ : ����
		Effect::PlayEffect(EFFECT_BOOM, _position);

		_SoundManager->Play("enemyDeath", 1.0f);
		//�������. �ٸ� �״� ����� ������ ��� �����Լ� ����� ���ؼ� ������ �ٲ� ���� �ִ�.
		this->Destroy();
	}
	//���� �ʾҴٸ�
	else
	{
		//�ǰ��� ���ߴٴ� ������ Ʈ��� ����� �ְ�
		_attacked = true;

		//ī�޶� ����
		_Camera->Shake();

		//������ ��Ʈ ��¿�
		_DamageFontManager->ShowDamage(_position, damage);

		//�ڷ� �з�����. �÷��̾��� �ޱ��� ���� �־��ָ� ������ �߰��ϸ� ���⿡�� �ݴ�� �ޱ۰��� �����Ƿ� �ݴ�������� �з��� �� �ִ�.
		this->_attackedAngle = Math::GetAngle(_player->GetPosition().x, _player->GetPosition().y, _position.x, _position.y);
	}
}

//�i�ų� �ǰݴ����� �� �����̱� ���� �Լ�.
//���Ŀ� �Ⱦ�� ����������
void Golem::Move()
{
	//�i����� �߰��� ���� �ޱ۰����� �̵��� ���� ������
	if (_state == StateType::Chasing)
	{
		//�i������ �ޱ۰�.
		this->_angle = Math::GetAngle(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);
		this->_position.x += cosf(_angle) * _speed * _TimeManager->DeltaTime();
		this->_position.y += -sinf(_angle)*_speed * _TimeManager->DeltaTime();
		this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);
	}

	//������ �¾����� ������� �̹���
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
