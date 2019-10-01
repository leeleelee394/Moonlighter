#include "stdafx.h"
#include "Player.h"

#include "Image.h"
#include "Animation.h"
#include "Timer.h"
#include "Enemy.h"
#include "MoveItem.h"
#include "Inventory.h"
#include "Effect.h"
#include "DamageFontManager.h"
#include "Boss.h"

using namespace Figure;


PlayerAni::PlayerAni()
{
}


PlayerAni::~PlayerAni()
{
}

/********************************************************************************/
//## Move ##
//Vector2 direction:방향
/********************************************************************************/
void Player::Move(Vector2 direction)
{
	//이동하지 않았다면 Float EQUAL에 소수점오차값으로 확인하여 이동을 하지 않았다면 return처리하여 빠져나간다.
	if (FLOAT_EQUAL(direction.x, 0.f) && FLOAT_EQUAL(direction.y, 0.f))
		return;

	//현 좌표는 방향*스피드*델타타임
	this->_position += direction.Normalize()*_speed*_TimeManager->DeltaTime();
	//이동했으니 정밀 충돌 렉트 위치도 갱신한다.
	_collisionRect = RectMakeCenter(_position, Vector2(30.0f, 40.0f));

	//mainRect의 위치도 갱신
	this->UpdateMainRect();

	

	//=======================================
	//오브젝트와 충돌(InterRect함수 사용)
	//=======================================
	const vector <class GameObject*>* object;
	object = _ObjectManager->GetObjectListPointer(ObjectType::Object);

	//모든 오브젝트를  for문으로 충돌 검사를 해준다.
	for (int i = 0; i < object->size(); i++)
	{
		//플레이어 자신을 제외하기 위한 조건문
		if (object->at(i)->GetName() != this->_name)
		{	
			//아이템 클래스 형변환으로 오브젝트 i로 불러온다.
			MoveItem* item = dynamic_cast<MoveItem*>(object->at(i));
			//에너미 클래스 형변환으로 오브젝트 i로 불러온다.
			Enemy* enemy = dynamic_cast<Enemy*>(object->at(i));				

			//다이나믹 캐스트로 null값이 반환되는 경우
			//플레이어 몸과 충돌검사
			//아이템(먹고 흡수해야해서 충돌x) //에너미(따로 검사?

			
			//아이템이 아닌 경우, 에너미가 아닌 경우->벽과 나무일 경우
			if (item == nullptr || enemy == nullptr)// || boss!=nullptr)
			{	
				//만든 함수 InterRee로 플레이어 충돌용 함수와 전체 오브젝트를 충돌 검사한다.
				if (this->InterRee(&_collisionRect, &object->at(i)->GetCollisionRect()))
				{
					//검사하는 오브젝트 i가 enemy일 경우, Roll 상태일때 통과하여 넘어간다.
					if (object->at(i)==enemy)
					{
						if (_state == Player::State::LeftRoll || _state == Player::State::RightRoll || _state == Player::State::UpRoll || _state == Player::State::DownRoll)
						{
							continue;
						}
					}
					//아이템인 경우 무조건 통과
					if(object->at(i)==item)
					{
						continue;
					}
					
					//충돌한 캐릭터 플레이어를 반대로 밀어주면서 그자리에 머문것처럼 한다.
					_position.x = (_collisionRect.right - _collisionRect.left) / 2 + _collisionRect.left;
					_position.y = (_collisionRect.bottom - _collisionRect.top) / 2 + _collisionRect.top;
					_mainRect = RectMakeCenter(_position.x, _position.y, _size.x, _size.y);
				}
			}
		}
	}
	//롤링시 통과하기 위한 bool값
	
	//에너미와의 충돌	
	//this->SendCallbackMessage(TagMessage("PlayerHP",0.0f, this->_currentHp));

	//Enemy* enemy = dynamic_cast<Enemy*>(object[i]);
	//Enemy 클래스를 가리키는 enemy에 담는다.
	//dynamic_case다이나믹 캐스(상속관계일 경우 형변환(ex: (float)hp ), 없으면 null값을 반환해줘서 편리!)
	//<Enemy*> 데이터 타입에... 에너미를 가리키는 클래스를 담았나?
	//해당 오브젝트 배열[i]로 호출
}


/********************************************************************************/
//## InterRee ##
//모든 오브젝트와 충돌 검사만을 위한 함수
/********************************************************************************/
bool Player::InterRee(RECT* moveRc, RECT* unMoveRc)
{
	RECT temp;
	if (!IntersectRect(&temp, moveRc, unMoveRc))
	{
		return false;
		//return false면 실행하지 않고 나가게 됨. 그래서 false는 먼저 적는게 좋음
		//reuturn을 만나면 나가는거 중요!
	}

	int tempWidth = temp.right - temp.left;
	int tempHeight = temp.bottom - temp.top;

	//가로길이가 더 넓다면 //상하로 부딪힌 경우이다.
	if (tempWidth > tempHeight)
	{
		//플레이어 상->하로 충돌
		if (moveRc->bottom == temp.bottom)
		{
			//충돌지점 길이만큼 플레이어에게 더해줘서 밑으로 내린다.
			//_position.y -= tempHeight;
			moveRc->top -= tempHeight;
			moveRc->bottom -= tempHeight;
		}

		//플레이어 하->상으로 충돌
		if (moveRc->top == temp.top)
		{
			//충돌지점 길이만큼 플레이어에게 빼줘서 위로 올린다.
			//_position.y += tempHeight;
			moveRc->top += tempHeight;
			moveRc->bottom += tempHeight;
		}
	}
	//만약 세로 길이가 더 길다면 //좌우로 부딪힌 경우이다
	else if (tempHeight > tempWidth)
	{
		//return false;
		//플레이어 우->좌로 충돌
		if (moveRc->left == temp.left)
		{
			//만약 플레이어 렉트가 충돌지점보다 오른쪽에서 있다면,템프길이만큼 더해준다.
			//_position.x += tempWidth;
			moveRc->left += tempWidth;
			moveRc->right += tempWidth;
		}

		//플레이어 좌->우로 충돌
		else if (moveRc->right == temp.right)
		{
			//_position.x -= tempWidth;
			moveRc->left -= tempWidth;
			moveRc->right -= tempWidth;
		}
	}
	return true;
	//return true
}

//=======================================
//플레이어 위치 전달 함수 (인벤용 XY값 전달)
//=======================================
POINT Player::GetPlayerIndex()
{
	POINT IndexXY = { _mainAnimation->GetNowFrameX(), _mainAnimation->GetNowFrameY() };
	return IndexXY;
}

//=======================================
//플레이어 검 + 에너미 = 에너미 체력 감소
//=======================================
void Player::Attack()
{
	if (_isAttacked == false)
	{
		const vector <class GameObject*>* object;
		object = _ObjectManager->GetObjectListPointer(ObjectType::Object);

		for (int i = 0; i < object->size(); i++)
		{
			//플레이어 자신을 제외하기 위한 조건문
			if (object->at(i)->GetName() != this->_name)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(object->at(i));
				Boss* boss = dynamic_cast<Boss*>(object->at(i));

				//무기와 에너미 충돌, 가져온 오브젝트에 값이 있을 경우만 검사
				if (enemy != nullptr)
				{
					RECT temp;
					if (IntersectRect(&temp, &_swordRect, &object->at(i)->GetCollisionRect()))
					{
						//데미지값을 받아서 체력을 깎는다(전달)
						enemy->AttackedDemege(_damage);		
						_SoundManager->Play("enemy_death", 0.6f);
						_isAttacked = true;
					}
				}
				
				if (boss != nullptr)
				{
					RECT temp;
					if (IntersectRect(&temp, &_swordRect, &object->at(i)->GetCollisionRect()))
					{
						//데미지값을 받아서 체력을 깎는다(전달)
						boss->AttackedDamage(_damage);
						_SoundManager->Play("enemy_death", 0.6f);
						_isAttacked = true;
					}
				}
			}

			if (_isAttacked == true)
			{
				//충돌 이펙트 발생
				Effect::PlayEffect(EFFECT_SWORDATK, Vector2(_swordRect.left, _swordRect.top));						
			}
		}
	}
}


//=======================================
//인벤토리 on/off 버튼용 함수
//=======================================
void Player::InventoryOnOff()
{
	_isMoveStop = false;
}

//=======================================
//에너미 무기 + 플레이어 몸체 = 플레이어 체력 감소
//=======================================
void Player::AttackedDamage(int damage)
{
	if (_isDelay == false)
	{
		//검사하는 오브젝트 i가 enemy일 경우, Roll 상태일때 통과하여 넘어간다.
		if (_state == Player::State::LeftRoll || _state == Player::State::RightRoll || _state == Player::State::UpRoll || _state == Player::State::DownRoll)
		{
			this->_currentHp -= 0;
		}		
		else
		{						
			_isDelay = true;
			_blink = 0;
			this->_currentHp -= damage;
			//데미지 폰트 출력
			_DamageFontManager->ShowDamage(Vector2(_position), damage);
			_SoundManager->Play("will_damage", 0.6f);
			_Camera->Shake();
		}
	}
}


//=======================================
//에너미와 충돌시, 무적시간 부여/플레이어 깜빡임 함수
//=======================================
void Player::AtkDelay2()
{
	if (_isDelay == true)
	{
		if (_currentHp > 0 && _state != Player::State::Die)
		{
			_count += _TimeManager->DeltaTime();
			
			if (_count > 0.2f)
			{
				_blink++;
				_count = 0;
				_alpha = !_alpha;
				_isDam = true;
				if (_blink == 6)
				{
					_isDelay = false;
					_alpha = 1.0f;
					_isDam = false;
				}
			}
		}
	}
}



	