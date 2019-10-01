#include "stdafx.h"
#include "Arrow.h"
#include "Image.h"
#include "Enemy.h"
#include "Effect.h"
#include "Boss.h"
#include "MoveItem.h"
#include "Player.h"

Arrow::Arrow(Vector2 pos, State state)
{
	this->_name = "arrow";
	this->_size = Vector2(10, 52);
	this->_position = pos;
	this->_speed = 400.0f;
	this->_damage = 20.f;		
	this->_state = state;
	this->_isColiArrow = false;
	_ImageManager->AddImage("arrow_left", L"../Resources/Player/arrow_left.png");
	this->_imgArrow_left = _ImageManager->FindImage("arrow_left");
	_ImageManager->AddImage("arrow_right", L"../Resources/Player/arrow_right.png");
	this->_imgArrow_right = _ImageManager->FindImage("arrow_right");
	_ImageManager->AddImage("arrow_up", L"../Resources/Player/arrow_up.png");
	this->_imgArrow_up = _ImageManager->FindImage("arrow_up");
	_ImageManager->AddImage("arrow_down", L"../Resources/Player/arrow_down.png");
	this->_imgArrow_down = _ImageManager->FindImage("arrow_down");
	//_mainRect = Figure::RectMakeCenter(_position, _size);
}

Arrow::~Arrow()
{
}

void Arrow::Init()
{
}

void Arrow::Release()
{
}

void Arrow::Update()
{
	if (_isColiArrow == false)
	{
		switch (_state)
		{
		case Arrow::State::Left:
			_position.x -= _speed * _TimeManager->DeltaTime();
			_size = Vector2(52, 10);
			this->_mainRect = Figure::RectMakeCenter(_position, _size);
			break;
		case Arrow::State::Right:
			_position.x += _speed * _TimeManager->DeltaTime();
			_size = Vector2(52, 10);
			this->_mainRect = Figure::RectMakeCenter(_position, _size);
			break;
		case Arrow::State::Up:
			_position.y -= _speed * _TimeManager->DeltaTime();
			_size = Vector2(10, 52);
			this->_mainRect = Figure::RectMakeCenter(_position, _size);
			break;
		case Arrow::State::Down:
			_position.y += _speed * _TimeManager->DeltaTime();
			_size = Vector2(10, 52);
			this->_mainRect = Figure::RectMakeCenter(_position, _size);
			break;
		}

		this->ArrowAttack();


		//화면 밖으로 나가면 삭제
		RECT renderRc = _Camera->GetRelativeRect(_mainRect);
		if (renderRc.left > WinSizeX + 100 || renderRc.right < -100 ||
			renderRc.top > WinSizeY + 100 || renderRc.bottom < -100)
		{
			this->Destroy();
		}

	}
	
	
}

void Arrow::Render()
{
	switch (_state)
	{
	case Arrow::State::Left:
		this->_size = Vector2(10, 52);
		_imgArrow_left->Render(_position.x, _position.y,Pivot::CENTER,true);
		break;
	case Arrow::State::Right:
		this->_size = Vector2(10, 52);
		_imgArrow_right->Render(_position.x, _position.y, Pivot::CENTER, true);
		break;
	case Arrow::State::Up:
		this->_size = Vector2(52, 10);
		_imgArrow_up->Render(_position.x, _position.y, Pivot::CENTER, true);
		break;
	case Arrow::State::Down:
		this->_size = Vector2(52, 10);
		_imgArrow_down->Render(_position.x, _position.y, Pivot::CENTER, true);
		break;
	}
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
	}	
}



//===================================
//화살 충돌 함수
//===================================
void Arrow::ArrowAttack()
{
	const vector<class GameObject*>* object;
	object = _ObjectManager->GetObjectListPointer(ObjectType::Object);

	for (UINT i = 0; i < object->size(); ++i)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(object->at(i));
		Boss* boss = dynamic_cast<Boss*>(object->at(i));
		Player* player = dynamic_cast<Player*>(object->at(i));
		MoveItem* item = dynamic_cast<MoveItem*>(object->at(i));

		//화살 자신과의 충돌을 제외함
		if (object->at(i)->GetName() != this->_name)// && player == nullptr)// && object->at(i)->GetName() !="WIll")
		{

			//에너미제외,보스제외,아이템 제외,플레이어 제외한 벽등의 오브젝트들
			RECT temp0;
			if (IntersectRect(&temp0, &this->_mainRect, &object->at(i)->GetCollisionRect()))
			{
				if (enemy == nullptr && boss == nullptr && item == nullptr && player == nullptr)
				{

					Effect::PlayEffect(EFFECT_BOWATK, Vector2(this->_mainRect.left, this->_mainRect.top));
					this->Destroy();
					break;
				}
			}


			//에너미일 경우
			if (enemy != nullptr)
			{
				RECT temp;
				if(IntersectRect(&temp, &this->_mainRect, &object->at(i)->GetCollisionRect()))
				{
					enemy->AttackedDemege(_damage);
					Effect::PlayEffect(EFFECT_BOWATK, Vector2(this->_mainRect.left, this->_mainRect.top));
					this->Destroy();
					break;
				}
				this->_isColiArrow = false;
			}

			//보스일 경우
			if (boss != nullptr)
			{
				RECT temp;
				if (IntersectRect(&temp, &this->_mainRect, &object->at(i)->GetCollisionRect()))
				{
					boss->AttackedDamage(_damage);
					Effect::PlayEffect(EFFECT_BOWATK, Vector2(this->_mainRect.left, this->_mainRect.top));
					this->Destroy();
					break;
				}
				this->_isColiArrow = false;
			}

			
		}
	}
}

//===================================
//화살 충돌 함수
//===================================
void Arrow::Coli()
{
	if (_isColiArrow == true)
	{
		Effect::PlayEffect(EFFECT_BOWATK, Vector2(this->_mainRect.left, this->_mainRect.top));

		//충돌한 캐릭터 플레이어를 반대로 밀어주면서 그자리에 머문것처럼 한다.
		this->_position.x = (this->_mainRect.right - this->_mainRect.left) / 2 + this->_mainRect.left;
		this->_position.y = (this->_mainRect.bottom - this->_mainRect.top) / 2 + this->_mainRect.top;
		this->_mainRect = Figure::RectMakeCenter(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}
}