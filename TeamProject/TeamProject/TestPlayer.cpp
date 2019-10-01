#include "stdafx.h"
#include "TestPlayer.h"

#include "Image.h"
#include "Animation.h"

#include "Timer.h"

TestPlayer::TestPlayer()
{
}


TestPlayer::~TestPlayer()
{
}

/***********************************************************************************
## Init ##
************************************************************************************/
void TestPlayer::Init()
{
	//이미지 추가 및 매니져로부터 찾아온다. 
	_ImageManager->AddFrameImage("Will", L"../Resources/Player/will_dungeon.png", 10, 13);
	this->_image = _ImageManager->FindImage("Will");
	//기본 변수들 초기화 
	this->_name = "TestPlayer";
	this->_size = Vector2(120, 120);
	this->_position = Vector2(700,935);
	this->_isActive = true;
	this->_pivot = Pivot::CENTER;
	this->_speed = 300.0f;
	this->UpdateMainRect();
	//정밀 충돌용 렉트 위치 초기화d
	this->_collisionRect = Figure::RectMakeCenter(_position, Vector2(60.f, 60.f));
	//상태별로 애니메이션 전부 생성해서 맵에 담아둔다. 
	this->CreateAnimation();
	//처음 시작 상태는 DownIdle로 
	this->ChangeState(State::DownIdle);
}
/***********************************************************************************
## Release ##
애니메이션은 이 객체가 생성했고 관리하므로 이곳에서 삭제해주어야 한다. 
************************************************************************************/
void TestPlayer::Release()
{
	AnimationIter iter = _animationList.begin();
	for (; iter != _animationList.end(); ++iter)
	{
		SafeDelete(iter->second);
	}
	_animationList.clear();
}
/***********************************************************************************
## Update ##

************************************************************************************/
void TestPlayer::Update()
{
	//이동량 측정할 변수
	Vector2 moveValue(0,0);
	//상태에 따라 다르게 업데이트
	switch (_state)
	{
	case TestPlayer::State::LeftIdle:
		//Idle 상태일 때 키 입력에 따라 상태 바뀌게 하는 함수( 중복 코드라서 함수로 빼놨습니다.)
		this->IdleKeyInput();
		break;
	case TestPlayer::State::RightIdle:
		//Idle 상태일 때 키 입력에 따라 상태 바뀌게 하는 함수( 중복 코드라서 함수로 빼놨습니다.)
		this->IdleKeyInput();
		break;
	case TestPlayer::State::UpIdle:
		//Idle 상태일 때 키 입력에 따라 상태 바뀌게 하는 함수( 중복 코드라서 함수로 빼놨습니다.)
		this->IdleKeyInput();
		break;
	case TestPlayer::State::DownIdle:
		//Idle 상태일 때 키 입력에 따라 상태 바뀌게 하는 함수( 중복 코드라서 함수로 빼놨습니다.)
		this->IdleKeyInput();
		break;
	case TestPlayer::State::LeftRun:
		//LeftRun 상태 인데 A키를 계속누르고 있다면 계속 이동 방향 왼쪽 추가해주고 
		if (_Input->GetKey('A'))
			moveValue += Vector2(-1.f, 0.f);
		//만약 LeftRun상태인데 A키에서 손을 땠다면 왼쪽 기본상태로 변경해주어야 한다. 
		else if (_Input->GetKeyUp('A'))
			this->ChangeState(State::LeftIdle);
		//왼쪽으로 달리는 상태인데 W나S를 눌렀다면 대각으로 이동해야 하므로 이동 방향벡터 추가 세팅
		if (_Input->GetKey('W'))
			moveValue += Vector2(0.f, -1.f);
		else if (_Input->GetKey('S'))
			moveValue += Vector2(0.f, 1.f);
		break;
		/*
		나머지 RightRun,UpRun,DownRun도 위와 같음
		*/

	case TestPlayer::State::RightRun:
		if (_Input->GetKey('D'))
			moveValue += Vector2(1.f, 0.f);
		else if (_Input->GetKeyUp('D'))
			this->ChangeState(State::RightIdle);

		if (_Input->GetKey('W'))
			moveValue += Vector2(0.f, -1.f);
		else if (_Input->GetKey('S'))
			moveValue += Vector2(0.f, 1.f);
		break;
	case TestPlayer::State::UpRun:
		if (_Input->GetKey('W'))
			moveValue += Vector2(0.f, -1.f);
		else if (_Input->GetKeyUp('W'))
			this->ChangeState(State::UpIdle);

		if (_Input->GetKey('A'))
			moveValue += Vector2(-1.f, 0.f);
		else if (_Input->GetKey('D'))
			moveValue += Vector2(1.f, 0.f);
		break;
	case TestPlayer::State::DownRun:
		if (_Input->GetKey('S'))
			moveValue += Vector2(0.f, 1.f);
		else if (_Input->GetKeyUp('S'))
			this->ChangeState(State::DownIdle);

		if (_Input->GetKey('A'))
			moveValue += Vector2(-1.f, 0.f);
		else if (_Input->GetKey('D'))
			moveValue += Vector2(1.f, 0.f);
		break;
		break;
	default:
		break;
	}
	//이동방향으로 이동 시킨다.
	this->Move(moveValue);
	//현재 애니메이션 Udpate시켜준다.
	_mainAnimation->UpdateFrame();

}
/***********************************************************************************
## Render ##
************************************************************************************/
void TestPlayer::Render()
{
	//이미지 사이즈 정해주고 
	_image->SetSize(_size);
	//렌더링
	_image->FrameRender((int)_position.x, _position.y, _mainAnimation->GetNowFrameX(),
		_mainAnimation->GetNowFrameY(), Pivot::CENTER, true);

	//디버그 모드라면 디버그 렉트들 렌더링(F1누르면 껐다 켜졌다 됌) 
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(_collisionRect, DefaultBrush::red, true);
	}

}
/***********************************************************************************
## ChangeState ##
상태를 바꿀 때는 _state = state;이런식으로 해주기 보다는 
이렇게 함수를 통해서 해준다. 이렇게 되면 디버깅 및 코드 관리가 쉬워지기 때문 

@@ State state : 바꿀 상태
************************************************************************************/
void TestPlayer::ChangeState(State state)
{
	//만약 현재 상태랑 바뀌어야 할 상태랑 같다면 아래 코드들을 실행시키지 말고 
	//함수를 빠져나가자 
	if (_state == state)
		return;

	//여기 까지 왔다면 상태를 바꿔도 된다면 현재 상태를 새로 바꿔야할 상태로 변경해준다.
	_state = state;
	//상태가 바뀌었으므로 애니메이션도 바꿔준다. 
	this->ChangeAnimation(state);
	//여기서는 상태가 바뀌었을 때 바뀐 상태에 따라 처리해 주어야 할 것들을 처리해준다. 
	switch (_state)
	{
	case TestPlayer::State::LeftIdle:
		break;
	case TestPlayer::State::RightIdle:
		break;
	case TestPlayer::State::UpIdle:
		break;
	case TestPlayer::State::DownIdle:
		break;
	case TestPlayer::State::LeftRun:
		break;
	case TestPlayer::State::RightRun:
		break;
	case TestPlayer::State::UpRun:
		break;
	case TestPlayer::State::DownRun:
		break;
	default:
		break;
	}
}

/**********************************************************************************
## Move ##
@@ Vector2 direction : 방향
************************************************************************************/
void TestPlayer::Move(Vector2 direction)
{
	//현재 좌표는 방향(정규화됀 값) * speed * DeltaTime
	this->_position += direction.Normalize() * _speed * _TimeManager->DeltaTime();
	//이동 했으므로 정밀 충돌용 렉트의 위치도 갱신해준다.
	_collisionRect = Figure::RectMakeCenter(_position + Vector2(0.f, 10.f), Vector2(60.f, 40.f));
	//mainRect의 위치도 갱신
	this->UpdateMainRect();

	const vector<GameObject*>* pObjectList = _ObjectManager->GetObjectListPointer(ObjectType::Object);
	for (UINT i = 0; i < pObjectList->size(); ++i)
	{
		GameObject* object = pObjectList->at(i);
		if (object != this)
		{
			if (Figure::IsCollisionReaction(_collisionRect, object->GetCollisionRect()) == true)
			{
				_position.x = CastingFloat(_collisionRect.left) + 30.f;
				_position.y = CastingFloat(_collisionRect.top) + 10.f;
				this->UpdateMainRect();
			}
		}
	}
}
/***********************************************************************************
## ChangeAnimation ##
상태에 따라 애니메이션을 바꿔준다.

@@ State state : 바꿀 애니메이션 키값 
************************************************************************************/
void TestPlayer::ChangeAnimation(State state)
{
	//반복자를 통해 맵에서 해당 키값의 애니메이션을 찾는다. 
	AnimationIter iter = _animationList.find(state);
	//만약 끝이 아니라면 찾았다는 뜻이므로 
	if (iter != _animationList.end())
	{
		//현재 애니메이션을 바꿀 애니메이션으로 교체 
		_mainAnimation = iter->second;
		_mainAnimation->Stop();
		_mainAnimation->Play();
	}
}

/***********************************************************************************
## CreateAnimation ##
애니메이션들 전부 미리 생성 및 초기화 해준후에 맵에 담아서 관리한다. 
************************************************************************************/
void TestPlayer::CreateAnimation()
{
	Animation* leftIdle = new Animation;
	leftIdle->SetStartEndFrame(0, 9, 9, 9, false);
	leftIdle->SetIsLoop(true);
	leftIdle->SetFrameUpdateTime(0.5f);
	_animationList.insert(make_pair(State::LeftIdle, leftIdle));

	Animation* rightIdle = new Animation;
	rightIdle->SetStartEndFrame(0, 8, 9, 8, false);
	rightIdle->SetIsLoop(true);
	rightIdle->SetFrameUpdateTime(0.5f);
	_animationList.insert(make_pair(State::RightIdle, rightIdle));

	Animation* upIdle = new Animation;
	upIdle->SetStartEndFrame(0, 10, 9, 10, false);
	upIdle->SetIsLoop(true);
	upIdle->SetFrameUpdateTime(0.5f);
	_animationList.insert(make_pair(State::UpIdle, upIdle));

	Animation* downIdle = new Animation;
	downIdle->SetStartEndFrame(0, 11, 9, 11, false);
	downIdle->SetIsLoop(true);
	downIdle->SetFrameUpdateTime(0.5f);
	_animationList.insert(make_pair(State::DownIdle, downIdle));

	Animation* leftRun = new Animation;
	leftRun->SetStartEndFrame(0, 3, 7, 3, false);
	leftRun->SetIsLoop(true);
	leftRun->SetFrameUpdateTime(0.08f);
	_animationList.insert(make_pair(State::LeftRun, leftRun));

	Animation* rightRun = new Animation;
	rightRun->SetStartEndFrame(0, 2, 7, 2, false);
	rightRun->SetIsLoop(true);
	rightRun->SetFrameUpdateTime(0.08f);
	_animationList.insert(make_pair(State::RightRun, rightRun));

	Animation* upRun = new Animation;
	upRun->SetStartEndFrame(0, 0, 7, 0, false);
	upRun->SetIsLoop(true);
	upRun->SetFrameUpdateTime(0.08f);
	_animationList.insert(make_pair(State::UpRun, upRun));

	Animation* downRun = new Animation;
	downRun->SetStartEndFrame(0, 1, 7, 1, false);
	downRun->SetIsLoop(true);
	downRun->SetFrameUpdateTime(0.08f);
	_animationList.insert(make_pair(State::DownRun, downRun));
}

/***********************************************************************************
## IdleKeyInput ##
Idle 상태일 때 키 입력 처리
************************************************************************************/
void TestPlayer::IdleKeyInput()
{
	if (_Input->GetKeyDown('A'))
		this->ChangeState(State::LeftRun);
	else if (_Input->GetKeyDown('D'))
		this->ChangeState(State::RightRun);
	else if (_Input->GetKeyDown('W'))
		this->ChangeState(State::UpRun);
	else if (_Input->GetKeyDown('S'))
		this->ChangeState(State::DownRun);
}