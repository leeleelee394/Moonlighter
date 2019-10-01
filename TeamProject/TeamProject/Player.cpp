#include "stdafx.h"
#include "Player.h"
#include "Image.h"
#include "Animation.h"
#include "Timer.h"
#include "Enemy.h"
#include "MoveItem.h"
#include "Inventory.h"
#include "Arrow.h"
#include "Effect.h"
#include "DamageFontManager.h"

//에너미 공ㅇ격 받을때 무적 판정(약 1초)	ok
//에너미 데미지 값 받을 함수				ok
//맞으면 알파값 깜빡임							ok		
//무기 보우, 방패
//한번만 칼 휘두르게
//두번 누를때 두번 휘두르게
//휘두르는 동안 제대로 렉트가 한번씩 먹히도록 수정한다.

//0310:보스 생성(최초) 이미지에서 뒷쪽으로 살짝 튀는 프레임 두어개쯤 있음
//대각선 롤링 완료
//칼 나누어서 휘두르기 어색해서 재수정

using namespace Figure;

//씬 변경으로 TItleScene에서 TitleObject의 TitleFlowObject.cpp
//23,27번 Dungeon_map_02 맵으로 수정함 

Player::Player(Vector2 pos)
{
	//이미지 추가, 매니저에서 FInd 하여 찾아옴
	_ImageManager->AddFrameImage("Will", L"../Resources/Player/will_dungeon.png", 10, 13);
	this->_imgMove = _ImageManager->FindImage("Will");
	_ImageManager->AddFrameImage("Will_Sword", L"../Resources/Player/will_sword.png", 11, 4);
	this->_imgAtkSword = _ImageManager->FindImage("Will_Sword");
	_ImageManager->AddFrameImage("Will_Bow", L"../Resources/Player/will_bow.png", 9, 4);
	this->_imgAtkBow = _ImageManager->FindImage("Will_Bow");
	_ImageManager->AddImage("Shadow", L"../Resources/Object/Shadow.png");
	this->_imgShadow = _ImageManager->FindImage("Shadow");

	//기본 변수 초기화
	this->_name = "Will";
	this->_size = Vector2(120, 120);
	this->_position = Vector2(627, 120);
	this->_position = pos;	//따로값 입력 안하면 메인신에서 값을 입력한대로
	this->_isActive = true;
	this->_pivot = Pivot::CENTER;
	this->_speed = 400.0f;
	this->UpdateMainRect();
	this->_maxHp = 100;
	this->_currentHp = 100;
	this->_isMoveStop = false;			//움직임을 멈추기 위한 bool값
	this->_isChangeSword = false;		//공격시 이미지 파일 변경을 위한 bool값
	this->_isChangeBow = false;			//활 공격시 이미지 파일 변경을 위한 bool값
	this->_isStandardMove = true;
	this->_damage = 25;					//플레이어 기본 무기 데미지

	this->_frameIdle = 0.1f;			//스피드 변경을 위한 변수
	this->_frameRun = 0.1f;				//스피드 변경을 위한 변수
	this->_frameBow = 0.08f;
	this->_swordWidth= 40;
	this->_swordHeight= 20;
	this->_isAttacked = false;
	this->_isChangeWeapon = false;		//false는 칼, true는 활
	this->_blink = 0;
	this->_alpha = 1.0f;
	this->_isDam = false;
	this->_isDelay = 0.f;
	this->_count = 0;
	//정밀 충돌용 렉트 위치 초기화
	this->_collisionRect = RectMakeCenter(_position, Vector2(30.0f, 40.0f));	
	//상태별 애니메이션 전부 생성하여 맵에 담아둔다.
	this->CreateAnimation();

	// 처음 시작 상태를 위한 세팅
	this->ChangeState(State::DownIdle);

	this->SetActive(_Database->GetIntData("PlayerActive"));
	_Database->AddIntData("PlayerActive", 1);
	this->_currentHp = _Database->GetIntData("PlayerHP");
}
Player::~Player() {}

/********************************************************************************/
//## Init ##
/********************************************************************************/
void Player::Init()
{	
	//AddCallbackMessage("InventoryOpen", [this](TagMessage message) {this->InventoryOnOff(); });
	AddCallbackMessage("InventoryClose", [this](TagMessage message) {this->InventoryOnOff(); });	
}

/********************************************************************************/
//## Release ##
/********************************************************************************/
void Player::Release()
{
	AnimationIter iter = _animationList.begin();
	for (; iter != _animationList.end(); ++iter)
	{
		SafeDelete(iter->second);
	}
	_animationList.clear();

	if (_currentHp <= 0)
	{
		_Database->AddIntData("PlayerHP", 100);
	}
	else
	{
		_Database->AddIntData("PlayerHP", this->_currentHp);
	}
}

/********************************************************************************/
//## Update ##
/********************************************************************************/
void Player::Update()
{	

	//%%%%%%%%%%%%%%%%%%-------------------------------치트키------------------------------%%%%%%%%%%%%%%%%%%%%%%%%%
	//나중에 지울 것:: 칫흐키 강한 플레이어
	if (_Input->GetKeyDown('E')) this->_currentHp = 100;
	//죽음
	if (_Input->GetKeyDown('R')) this->_currentHp = 0;
	//%%%%%%%%%%%%%%%%%%-------------------------------치트키------------------------------%%%%%%%%%%%%%%%%%%%%%%%%%
	
	//플레이어 죽음
	if (_currentHp <= 0)
	{
		this->ChangeState(Player::State::Die);
	}

	//이동량 측정할 변수
	Vector2 moveValue(0, 0);
	
	
		if (_Input->GetKeyDown('I'))
		{
			GameObject* _inventory = (Inventory*)_ObjectManager->FindObject(ObjectType::UI, "Inventory");
			
			if (_inventory->GetActive() == false)
			{
				if (_state == Player::State::LeftRun || _state == Player::State::LeftRoll || _state == Player::State::LeftSword1 || _state == Player::State::LeftSword2 || _state == Player::State::LeftBow)
				{
					_state = Player::State::LeftIdle;
				}
				else if (_state == Player::State::RightRun || _state == Player::State::RightRoll || _state == Player::State::RightSword1 || _state == Player::State::RightSword2 || _state == Player::State::RightBow)
				{
					_state = Player::State::RightIdle;
				}
				else if (_state == Player::State::UpRun || _state == Player::State::UpRoll || _state == Player::State::UpSword1 || _state == Player::State::UpSword2 || _state == Player::State::UpBow)
				{
					_state = Player::State::UpIdle;
				}
				else if (_state == Player::State::DownRun || _state == Player::State::DownRoll || _state == Player::State::DownSword1 || _state == Player::State::DownSword2 || _state == Player::State::DownBow)
				{
					_state = Player::State::DownIdle;
				}

				_isMoveStop = !_isMoveStop;
				_inventory->SetActive(true);
			} 
		}

		//무기 변경(칼<->활)
		if (_Input->GetKeyDown('Z'))
		{
			_isChangeBow = !_isChangeBow;
		}

		//인벤토리  on/off용
		{
			//상태에 따라 다르게 업데이트
			switch (_state)
			{
			case Player::State::LeftIdle:
				if (_isMoveStop == false)
				{
					if (_Input->GetKey('A')) this->ChangeState(State::LeftRun);
					else if (_Input->GetKey('D')) this->ChangeState(State::RightRun);
					else if (_Input->GetKey('W')) this->ChangeState(State::UpRun);
					else if (_Input->GetKey('S')) this->ChangeState(State::DownRun);
					else if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::LeftRoll);
					else if (!_isChangeBow && _Input->GetKey('J')) this->ChangeState(State::LeftSword1);
					else if (_isChangeBow && _Input->GetKey('J')) this->ChangeState(State::LeftBow);
				}				
				break;

			case Player::State::RightIdle:
				if (_isMoveStop == false)
				{
					if (_Input->GetKey('A')) this->ChangeState(State::LeftRun);
					else if (_Input->GetKey('D')) this->ChangeState(State::RightRun);
					else if (_Input->GetKey('W')) this->ChangeState(State::UpRun);
					else if (_Input->GetKey('S')) this->ChangeState(State::DownRun);
					else if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::RightRoll);
					else if (!_isChangeBow && _Input->GetKey('J')) this->ChangeState(State::RightSword1);
					else if (_isChangeBow && _Input->GetKey('J')) this->ChangeState(State::RightBow);
				}
				break;

			case Player::State::UpIdle:
				if (_isMoveStop == false)
				{
					if (_Input->GetKey('A')) this->ChangeState(State::LeftRun);
					else if (_Input->GetKey('D')) this->ChangeState(State::RightRun);
					else if (_Input->GetKey('W')) this->ChangeState(State::UpRun);
					else if (_Input->GetKey('S')) this->ChangeState(State::DownRun);
					else if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::UpRoll);
					else if (!_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::UpSword1);
					else if (_isChangeBow&& _Input->GetKey('J')) this->ChangeState(State::UpBow);
				}
				break;

			case Player::State::DownIdle:
				if (_isMoveStop == false)
				{
					if (_Input->GetKey('A')) this->ChangeState(State::LeftRun);
					else if (_Input->GetKey('D')) this->ChangeState(State::RightRun);
					else if (_Input->GetKey('W')) this->ChangeState(State::UpRun);
					else if (_Input->GetKey('S')) this->ChangeState(State::DownRun);
					else if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::DownRoll);
					else if (!_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::DownSword1);
					else if (_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::DownBow);
				}
				break;

			case Player::State::LeftRun:
				if (_Input->GetKey('A')) moveValue += Vector2(-1.0f, 0.0f);		//왼누르기
				if (_Input->GetKeyUp('A')) this->ChangeState(State::LeftIdle); //왼떼기

				//대각선 시작		
				if (_Input->GetKey('W'))										//대각선 위 누르기
				{
					ChangeAnimation(Player::State::UpRun);
					moveValue += Vector2(0.0f, -1.0f);
				}
				else if (_Input->GetKey('S'))									//대각선 아래 누르기
				{
					moveValue += Vector2(0.0f, 1.0f);
					ChangeAnimation(Player::State::DownRun);
				}

				if (_Input->GetKeyUp('W'))			//대각선 위 떼기
				{
					ChangeAnimation(Player::State::LeftRun);
				}
				else if (_Input->GetKeyUp('S'))			//대각선 아래 떼기
				{
					this->ChangeState(State::LeftRun);
				}

				if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::LeftRoll);
				if (!_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::LeftSword1);
				else if (_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::LeftBow);
				break;

			case Player::State::RightRun:
				if (_Input->GetKey('D')) moveValue += Vector2(1.0f, 0.0f);
				if (_Input->GetKeyUp('D')) this->ChangeState(State::RightIdle);

				//대각선 시작
				if (_Input->GetKey('W'))	//대각선 위
				{
					ChangeAnimation(Player::State::UpRun);
					moveValue += Vector2(0.0f, -1.0f);
				}
				else if (_Input->GetKey('S'))	//대각선 아래
				{
					moveValue += Vector2(0.0f, 1.0f);
					ChangeAnimation(Player::State::DownRun);
				}

				if (_Input->GetKeyUp('W'))
				{
					ChangeAnimation(Player::State::RightRun);
				}
				else if (_Input->GetKeyUp('S'))
				{
					ChangeAnimation(Player::State::RightRun);
				}
				if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::RightRoll);
				if (!_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::RightSword1);
				else if (_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::RightBow);
				break;

			case Player::State::UpRun:
				if (_Input->GetKey('W')) moveValue += Vector2(0.0f, -1.0f);
				if (_Input->GetKeyUp('W')) this->ChangeState(State::UpIdle);

				if (_Input->GetKey('A')) moveValue += Vector2(-1.0f, 0.0f);
				else if (_Input->GetKey('D')) moveValue += Vector2(1.0f, 0.0f);

				if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::UpRoll);
				if (!_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::UpSword1);
				else if (_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::UpBow);
				break;

			case Player::State::DownRun:
				if (_Input->GetKey('S')) moveValue += Vector2(0.0f, 1.0f);
				if (_Input->GetKeyUp('S')) this->ChangeState(State::DownIdle);

				if (_Input->GetKey('A')) moveValue += Vector2(-1.0f, 0.0f);
				else if (_Input->GetKey('D')) moveValue += Vector2(1.0f, 0.0f);

				if (_Input->GetKeyDown(VK_SPACE)) this->ChangeState(State::DownRoll);
				if (!_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::DownSword1);
				else if (_isChangeBow&&_Input->GetKey('J')) this->ChangeState(State::DownBow);
				break;

				//=====================================================================================
			case Player::State::LeftRoll:
				moveValue += Vector2(-2.0f, 0.0f);

				//대각선 시작		
				if (_Input->GetKey('W'))										//대각선 위 누르기
				{
					moveValue += Vector2(0.0f, -2.0f);
				}
				else if (_Input->GetKey('S'))										//대각선 위 누르기
				{
					moveValue += Vector2(0.0f, 2.0f);
				}
				break;

			case Player::State::RightRoll:
				moveValue += Vector2(2.0f, 0.0f);

				//대각선 시작		
				if (_Input->GetKey('W'))										//대각선 위 누르기
				{
					moveValue += Vector2(0.0f, -2.0f);
				}
				else if (_Input->GetKey('S'))										//대각선 위 누르기
				{
					moveValue += Vector2(0.0f, 2.0f);
				}
				break;

			case Player::State::UpRoll:
				moveValue += Vector2(0.0f, -2.0f);

				//대각선 시작		
				if (_Input->GetKey('A'))										//대각선 위 누르기
				{
					moveValue += Vector2(-2.0f, 0.0f);
				}
				else if (_Input->GetKey('D'))										//대각선 위 누르기
				{
					moveValue += Vector2(2.0f, 0.0f);
				}
				break;

			case Player::State::DownRoll:
				moveValue += Vector2(0.0f, 2.0f);

				//대각선 시작		
				if (_Input->GetKey('A'))										//대각선 위 누르기
				{
					moveValue += Vector2(-2.0f, 0.0f);
				}
				else if (_Input->GetKey('D'))										//대각선 위 누르기
				{
					moveValue += Vector2(2.0f, 0.0f);
				}
				break;
				//=====================================================================================
			case Player::State::LeftSword1:
				this->Attack();
				break;

			case Player::State::RightSword1:
				this->Attack();
				break;

			case Player::State::UpSword1:
				this->Attack();
				break;

			case Player::State::DownSword1:
				this->Attack();
				break;
				//=====================================================================================
			case Player::State::LeftSword2:
				this->Attack();
				break;

			case Player::State::RightSword2:
				this->Attack();
				break;

			case Player::State::UpSword2:
				this->Attack();
				break;

			case Player::State::DownSword2:
				this->Attack();
				break;
				//=====================================================================================
			case Player::State::LeftBow:
				//this->Enable();
				break;

			case Player::State::RightBow:
				//this->Enable();
				break;

			case Player::State::UpBow:
				//this->Enable();
				break;

			case Player::State::DownBow:
				//this->Enable();
				break;

			case Player::State::Die:
				break;




			default:
				break;
			}
		}
		//이동 및 충돌관련 함수 호출함
		this->Move(moveValue);
	

	//프레임을 업데이트 해준다.
	_mainAnimation->UpdateFrame();
	
	//공격 판정 이후 처리 함수
	this->AtkDelay2();

}

/********************************************************************************/
//## Render ##
/********************************************************************************/
void Player::Render()
{
	//그림자
	_imgShadow->SetAlpha(0.4f);
	_imgShadow->SetSize(Vector2(48, 24));
	_imgShadow->Render(this->_position.x, this->_position.y+24, Pivot::CENTER,true);
	   

	//이미지 사이즈 지정
	_imgMove->SetSize(_size);
	_imgAtkSword->SetSize(_size);
	_imgAtkBow->SetSize(_size);
	_imgMove->SetAlpha(_alpha);
		

	//렌더링: 두개의 이미지를 상황에 맞게 번갈아가면서 사용하도록 조건을 준다.
	//무기 교체만 하는 상태에도 기본 무브 사용해야 하므로 공격 버튼 누를때만 렌더하도록 다른 조건을 줄 것

	if (_isStandardMove == true)
	{		
		_imgMove->FrameRender((int)_position.x, _position.y, _mainAnimation->GetNowFrameX(), _mainAnimation->GetNowFrameY(), Pivot::CENTER, true);
	}
	else if (_isChangeSword == true)
	{		
		_imgAtkSword->FrameRender((int)_position.x, _position.y, _mainAnimation->GetNowFrameX(), _mainAnimation->GetNowFrameY(), Pivot::CENTER, true);
	}	
	else if (_isChangeBow == true)
	{
		_imgAtkBow->FrameRender((int)_position.x, _position.y, _mainAnimation->GetNowFrameX(), _mainAnimation->GetNowFrameY(), Pivot::CENTER, true);
	}
	
	//디버그 모드라면 디버그 렉트들 렌더링 (F1)
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(_collisionRect, DefaultBrush::red, true);
		//공격모션(이미지)가 true이고 공격판정이 false일때만 그린다
		
		if (_isChangeSword && !_isAttacked)_DXRenderer->DrawRectangle(_swordRect, DefaultBrush::green, true);
	}
}

/********************************************************************************/
//## ChangeState ##
//상태를 바꿀 때는 _state=state; 보다는 직접 함수를 통해서 변경하면 
//디버깅 및 코드 관리가 쉬워진다.
/********************************************************************************/
void Player::ChangeState(State state)
{
	//현 상태와 동일하다면 실행시키지 않고 빠져나간다
	if (_state == state) return;

	//현 상태와 동일하지 않다면 변경한다.
	_state = state;
	//상태가 바뀌면서 애니메이션도 변경한다
	this->ChangeAnimation(state);
		
	//상태가 바뀌는 순간 처리할 ..사항을 적기 위함
	switch (_state)
		{
		case Player::State::LeftIdle:
			_isChangeSword = false;				//검공격 이미지 렌더를 위한 bool
			_isStandardMove = true;				//기본 무브 이미지 렌더 bool
			break;
		case Player::State::RightIdle:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
		case Player::State::UpIdle:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
		case Player::State::DownIdle:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
		case Player::State::LeftRun:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
		case Player::State::RightRun:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
		case Player::State::UpRun:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
		case Player::State::DownRun:
			_isChangeSword = false;
			_isStandardMove = true;
			break;
			//======================================================
		case Player::State::LeftRoll:
			_isChangeSword = false;
			_isStandardMove = true;
			_SoundManager->Play("will_roll", 0.6f);
			break;
		case Player::State::RightRoll:
			_isChangeSword = false;
			_isStandardMove = true;
			_SoundManager->Play("will_roll", 0.6f);
			break;
		case Player::State::UpRoll:
			_isChangeSword = false;
			_isStandardMove = true;
			_SoundManager->Play("will_roll", 0.6f);
			break;
		case Player::State::DownRoll:
			_isChangeSword = false;
			_isStandardMove = true;
			_SoundManager->Play("will_roll", 0.6f);
			break;
			//======================================================
		case Player::State::LeftSword1:
			this->_damage = 25;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x - 40, _position.y, _swordWidth, _swordHeight); //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
		case Player::State::RightSword1:
			this->_damage = 25;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x + 40, _position.y, _swordWidth, _swordHeight); //칼 렉트 
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
		case Player::State::UpSword1:
			this->_damage = 25;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x, _position.y - 40, _swordHeight, _swordWidth); //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
		case Player::State::DownSword1:
			this->_damage = 25;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x, _position.y + 40, _swordHeight, _swordWidth); //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
			//======================================================
		case Player::State::LeftSword2:
			this->_damage = 50;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x - 40, _position.y, _swordWidth, _swordHeight); //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
		case Player::State::RightSword2:
			this->_damage = 50;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x + 40, _position.y, _swordWidth, _swordHeight);  //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
		case Player::State::UpSword2:
			this->_damage = 50;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x, _position.y - 40, _swordHeight, _swordWidth);  //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
		case Player::State::DownSword2:
			this->_damage = 50;
			_isChangeSword = true;
			_isAttacked = false;
			_isStandardMove = false;
			if (_isChangeSword)this->_swordRect = RectMakeCenter(_position.x, _position.y + 40, _swordHeight, _swordWidth);  //칼 렉트
			_SoundManager->Play("will_swordAtk", 0.6f);
			break;
			//======================================================
		case Player::State::LeftBow:
			this->_damage = 20;
			_isChangeSword = false;
			_isStandardMove = false;
			_ObjectManager->AddObject(ObjectType::Object, new Arrow(Vector2(_position), Arrow::State::Left));
			_SoundManager->Play("will_bowAtk", 0.6f);
			break;

		case Player::State::RightBow:
			this->_damage = 20;
			_isChangeSword = false;
			_isStandardMove = false;
			_ObjectManager->AddObject(ObjectType::Object, new Arrow(Vector2(_position), Arrow::State::Right));
			_SoundManager->Play("will_bowAtk", 0.6f);
			break;

		case Player::State::UpBow:
			this->_damage = 20;
			_isChangeSword = false;
			_isStandardMove = false;
			_ObjectManager->AddObject(ObjectType::Object, new Arrow(Vector2(_position), Arrow::State::Up));
			_SoundManager->Play("will_bowAtk", 0.6f);
			break;

		case Player::State::DownBow:
			this->_damage = 20;
			_isChangeSword = false;
			_isStandardMove = false;
			_ObjectManager->AddObject(ObjectType::Object, new Arrow(Vector2(_position), Arrow::State::Down));
			_SoundManager->Play("will_bowAtk", 0.6f);
			break;

		case Player::State::Die:
			_SoundManager->Play("will_die", 0.6f);
			break;


		default:
			break;
		}
}



/********************************************************************************/
//## ChangeAnimation ##
//상태에 따라 애니메이션을 바꿔준다. (키값은 State state)
/********************************************************************************/
void Player::ChangeAnimation(State state)
{
	//반복자를 통해 맵에서 해당 키값의 애니메이션을 찾는다.
	AnimationIter iter = _animationList.find(state);

	//만약 끝이 아니면 찾은 것이니 현 애니메이션을 바꿀 애니메이션으로 교체한다.
	if (iter != _animationList.end())
	{
		if (iter->second != _mainAnimation)
		{
			_mainAnimation = iter->second;
			_mainAnimation->Stop();
			_mainAnimation->Play();
		}
	}
}

/********************************************************************************/
//## CreateAnimation ##
//애니메이션 전부 미리 생성 및 초기화 해준 후 맵에 담아서 관리
/********************************************************************************/
void Player::CreateAnimation()
{
	Animation* leftIdle = new Animation;							//상태별로 하나씩 new로 맹들었지렁
	leftIdle->SetStartEndFrame(0, 9, 9, 9, false);					//한동작의 시작x,y,끝x,y 저억으시오
	leftIdle->SetIsLoop(true);										//루프돌릴지 true, faㅣse
	leftIdle->SetFrameUpdateTime(_frameIdle);						//프레임당 시간을 넣어주면 됨미다
	_animationList.insert(make_pair(State::LeftIdle, leftIdle));	//값 다 만드셨으면 inset로 맵에 콱 박으쇼

	Animation* rightIdle = new Animation;
	rightIdle->SetStartEndFrame(0, 8, 9, 8, false);
	rightIdle->SetIsLoop(true);
	rightIdle->SetFrameUpdateTime(_frameIdle);
	_animationList.insert(make_pair(State::RightIdle, rightIdle));

	Animation* upIdle = new Animation;
	upIdle->SetStartEndFrame(0, 10, 9, 10, false);
	upIdle->SetIsLoop(true);
	upIdle->SetFrameUpdateTime(_frameIdle);
	_animationList.insert(make_pair(State::UpIdle, upIdle));

	Animation* downIdle = new Animation;
	downIdle->SetStartEndFrame(0, 11, 9, 11, false);
	downIdle->SetIsLoop(true);
	downIdle->SetFrameUpdateTime(_frameIdle);
	_animationList.insert(make_pair(State::DownIdle, downIdle));

	Animation* leftRun = new Animation;
	leftRun->SetStartEndFrame(0, 3, 7, 3, false);
	leftRun->SetIsLoop(true);
	leftRun->SetFrameUpdateTime(_frameRun);
	_animationList.insert(make_pair(State::LeftRun, leftRun));

	Animation* rightRun = new Animation;
	rightRun->SetStartEndFrame(0, 2, 7, 2, false);
	rightRun->SetIsLoop(true);
	rightRun->SetFrameUpdateTime(_frameRun);
	_animationList.insert(make_pair(State::RightRun, rightRun));

	Animation* upRun = new Animation;
	upRun->SetStartEndFrame(0, 0, 7, 0, false);
	upRun->SetIsLoop(true);
	upRun->SetFrameUpdateTime(_frameRun);
	_animationList.insert(make_pair(State::UpRun, upRun));

	Animation* downRun = new Animation;
	downRun->SetStartEndFrame(0, 1, 7, 1, false);
	downRun->SetIsLoop(true);
	downRun->SetFrameUpdateTime(_frameRun);
	_animationList.insert(make_pair(State::DownRun, downRun));

	Animation* leftRoll = new Animation;
	leftRoll->SetStartEndFrame(0, 5, 7, 5, false);
	leftRoll->SetIsLoop(false);
	leftRoll->SetFrameUpdateTime(_frameRun);	
	leftRoll->SetCallbackFunc([this]() {this->EndAnimation(); });	
	_animationList.insert(make_pair(State::LeftRoll, leftRoll));

	Animation* rightRoll = new Animation;
	rightRoll->SetStartEndFrame(0, 4, 7, 4, false);
	rightRoll->SetIsLoop(false);
	rightRoll->SetFrameUpdateTime(_frameRun);
	rightRoll->SetCallbackFunc([this]() {this->EndAnimation(); });	//람다식 함수 호출
	_animationList.insert(make_pair(State::RightRoll, rightRoll));

	Animation* upRoll = new Animation;
	upRoll->SetStartEndFrame(0, 6, 7, 6, false);
	upRoll->SetIsLoop(false);
	upRoll->SetFrameUpdateTime(_frameRun);
	upRoll->SetCallbackFunc([this]() {this->EndAnimation(); });		//프레임이 다 돌면 종료한다
	_animationList.insert(make_pair(State::UpRoll, upRoll));

	Animation* downRoll = new Animation;
	downRoll->SetStartEndFrame(0, 7, 7, 7, false);
	downRoll->SetIsLoop(false);
	downRoll->SetFrameUpdateTime(_frameRun);
	downRoll->SetCallbackFunc([this]() {this->EndAnimation(); });	//이 방식은 public에 선언된 애만 가능해!
	_animationList.insert(make_pair(State::DownRoll, downRoll)); 
	//======================================================================
	Animation* leftSword1 = new Animation;
	leftSword1->SetStartEndFrame(0, 3, 4, 3, false);
	leftSword1->SetIsLoop(false);
	leftSword1->SetFrameUpdateTime(_frameRun);
	leftSword1->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::LeftSword1, leftSword1));

	Animation* rightSword1 = new Animation;
	rightSword1->SetStartEndFrame(0, 2, 4, 2, false);
	rightSword1->SetIsLoop(false);
	rightSword1->SetFrameUpdateTime(_frameRun);
	rightSword1->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::RightSword1, rightSword1));

	Animation* upSword1 = new Animation;
	upSword1->SetStartEndFrame(0, 0, 4, 0, false);
	upSword1->SetIsLoop(false);
	upSword1->SetFrameUpdateTime(_frameRun);
	upSword1->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::UpSword1, upSword1));

	Animation* downSword1 = new Animation;	
	downSword1->SetStartEndFrame(0, 1, 4, 1, false);
	downSword1->SetIsLoop(false);
	downSword1->SetFrameUpdateTime(_frameRun);
	downSword1->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::DownSword1, downSword1));
	//=======================================================================
	Animation* leftSword2 = new Animation;
	leftSword2->SetStartEndFrame(5, 3, 10, 3, false);
	leftSword2->SetIsLoop(false);
	leftSword2->SetFrameUpdateTime(_frameRun);
	leftSword2->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::LeftSword2, leftSword2));

	Animation* rightSword2 = new Animation;
	rightSword2->SetStartEndFrame(5, 2, 10, 2, false);
	rightSword2->SetIsLoop(false);
	rightSword2->SetFrameUpdateTime(_frameRun);
	rightSword2->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::RightSword2, rightSword2));

	Animation* upSword2 = new Animation;
	upSword2->SetStartEndFrame(5, 0, 10, 0, false);
	upSword2->SetIsLoop(false);
	upSword2->SetFrameUpdateTime(_frameRun);
	upSword2->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::UpSword2, upSword2));

	Animation* downSword2 = new Animation;
	downSword2->SetStartEndFrame(5, 1, 10, 1, false);
	downSword2->SetIsLoop(false);
	downSword2->SetFrameUpdateTime(_frameRun);
	downSword2->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::DownSword2, downSword2));
	//=====================================================================================
	Animation* leftBow = new Animation;
	leftBow->SetStartEndFrame(0, 3, 8, 3, false);
	leftBow->SetIsLoop(false);
	leftBow->SetFrameUpdateTime(_frameBow);
	leftBow->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::LeftBow, leftBow));

	Animation* rightBow = new Animation;
	rightBow->SetStartEndFrame(0, 2, 8, 2, false);
	rightBow->SetIsLoop(false);
	rightBow->SetFrameUpdateTime(_frameBow);
	rightBow->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::RightBow, rightBow));

	Animation* upBow = new Animation;
	upBow->SetStartEndFrame(0, 0, 8, 0, false);
	upBow->SetIsLoop(false);
	upBow->SetFrameUpdateTime(_frameBow);
	upBow->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::UpBow, upBow));

	Animation* downBow = new Animation;
	downBow->SetStartEndFrame(0, 1, 8, 1, false);
	downBow->SetIsLoop(false);
	downBow->SetFrameUpdateTime(_frameBow);
	downBow->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::DownBow, downBow));

	Animation* die = new Animation;
	die->SetStartEndFrame(0, 12, 9, 12, false);
	die->SetIsLoop(false);
	die->SetFrameUpdateTime(0.2f);	
	die->SetCallbackFunc([this]() {this->EndAnimation(); });
	_animationList.insert(make_pair(State::Die, die));
}

//해당 상태 종료 후 변경할 상태 
void Player::EndAnimation()
{
	const vector<GameObject*>* objec = nullptr;
	switch (_state)
	{
	case Player::State::LeftIdle:
		break;
	case Player::State::RightIdle:
		break;
	case Player::State::UpIdle:
		break;
	case Player::State::DownIdle:
		break;
	case Player::State::LeftRun:
		break;
	case Player::State::RightRun:
		break;
	case Player::State::UpRun:
		break;
	case Player::State::DownRun:
		break;
	case Player::State::LeftRoll:
		if(_Input->GetKey('A')) this->ChangeState(State::LeftRun);
		else					this->ChangeState(State::LeftIdle);		
		break;
	case Player::State::RightRoll:
		if (_Input->GetKey('D')) this->ChangeState(State::RightRun);
		else					this->ChangeState(State::RightIdle);
		break;
	case Player::State::UpRoll:
		if (_Input->GetKey('W')) this->ChangeState(State::UpRun);
		else					this->ChangeState(State::UpIdle);
		break;
	case Player::State::DownRoll:
		if (_Input->GetKey('S')) this->ChangeState(State::DownRun);
		else					this->ChangeState(State::DownIdle);
		break;
		//================================================================
	case Player::State::LeftSword1:
		if (_Input->GetKey('J')) this->ChangeState(State::LeftSword2);
		else if (_Input->GetKeyDown('J')) this->ChangeState(State::LeftSword2);
		else if (_Input->GetKey('A')) this->ChangeState(State::LeftRun);
		else					this->ChangeState(State::LeftIdle);
		break;
	case Player::State::RightSword1:
		if (_Input->GetKey('J')) this->ChangeState(State::RightSword2);
		else if (_Input->GetKeyDown('J')) this->ChangeState(State::RightSword2);
		else if (_Input->GetKey('D')) this->ChangeState(State::RightRun);
		else					this->ChangeState(State::RightIdle);
		break;
	case Player::State::UpSword1:
		if (_Input->GetKey('J')) this->ChangeState(State::UpSword2);
		else if (_Input->GetKeyDown('J')) this->ChangeState(State::UpSword2);
		else if (_Input->GetKey('W')) this->ChangeState(State::UpRun);
		else					this->ChangeState(State::UpIdle);
		break;
	case Player::State::DownSword1:
		if (_Input->GetKey('J')) this->ChangeAnimation(State::DownSword2);
		else if (_Input->GetKeyDown('J')) this->ChangeState(State::DownSword2);
		else if (_Input->GetKey('S')) this->ChangeState(State::DownRun);
		else					this->ChangeState(State::DownIdle);
		break;
		//================================================================
	case Player::State::LeftSword2:
		this->ChangeState(State::LeftIdle);
		break;
	case Player::State::RightSword2:
		this->ChangeState(State::RightIdle);
		break;
	case Player::State::UpSword2:
		this->ChangeState(State::UpIdle);
		break;
	case Player::State::DownSword2:
		this->ChangeState(State::DownIdle);
		break;
		//==============================================================
	case Player::State::LeftBow:
		this->ChangeState(State::LeftIdle);
		break;

	case Player::State::RightBow:
		this->ChangeState(State::RightIdle);
		break;

	case Player::State::UpBow:
		this->ChangeState(State::UpIdle);
		break;

	case Player::State::DownBow:
		this->ChangeState(State::DownIdle);
		break;

	case Player::State::Die:
	{
		objec = _ObjectManager->GetObjectListPointer(ObjectType::UI);
		for (UINT i = 0; i < objec->size(); ++i)
		{
			objec->at(i)->SendCallbackMessage(TagMessage("PlayerDie"));
		}
		
		_Database->AddVector2Data("PlayerPosition", Vector2(716, 996));
		_Database->AddIntData("PlayerActive", 0);
		_Database->AddIntData("GateState", 5);
		_SoundManager->FadeoutBGM();
		_SceneManager->LoadSceneByLoading("DungeonLobby");
	
	}
	break;
	default:
		break;
	}
}
