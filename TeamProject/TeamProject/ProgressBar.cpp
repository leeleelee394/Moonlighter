#include "stdafx.h"
#include "ProgressBar.h"
#include "Player.h"

ProgressBar::ProgressBar()
{
}


ProgressBar::~ProgressBar()
{
}

void ProgressBar::Init()
{
	//HP바 이미지 프레임렌더
	_progressBarBackIMG = _ImageManager->FindImage("hpBar1");
	_progressBarFrontIMG = _ImageManager->FindImage("hpBar2");

	//하트 이미지 렌더
	_heartIMG = _ImageManager->AddImage("heart", L"../Resources/UI/heart.png", false);

	//클래스 키 이름 선언
	this->_name = "ProgressBar";

	//포지션 초기화
	this->_position = Vector2(140, 15);
	//렉트 생성
	this->_mainRect = Figure::RectMake(_position.x, _position.y, _progressBarBackIMG->GetWidth(), _progressBarBackIMG->GetHeight());

	//이미지 프레임 초기화 
	_imgFrameX = 0;
	_imgFrameY = 1;

	//세이브 hp 는 currentHp;
	Player* _player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");
	_saveHp = _player->GetPlayerCurrentHp();

	//디폴트 상태
	_barState = ProgressState::DefaultState;

	SetGauge(_player->GetPlayerCurrentHp(), _player->GetPlayerMaxHp());
	this->_hpWidth = ((float)_saveHp / (float)_player->GetPlayerMaxHp()) * (float)_progressBarFrontIMG->GetWidth();
}

void ProgressBar::Release()
{
}

void ProgressBar::Update()
{
	GameObject::Update();

	//메인 렉트 선언
	this->_mainRect = Figure::RectMake(_position.x, _position.y, _progressBarBackIMG->GetWidth(), _progressBarBackIMG->GetHeight());

	//플레이어 클래스 접근해서 플레이어 함수 가져오기
	Player* _player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");


	//변화한 HP바 길이 얻기
	SetGauge(_player->GetPlayerCurrentHp(), _player->GetPlayerMaxHp());

	//프로그래스 바 상태 함수
	ProgressBarState();

	//업데이트 렉트
	this->UpdateMainRect();
}

void ProgressBar::Render()
{
	//HP바 테두리 이미지
	_progressBarBackIMG->SetSize(Vector2(_progressBarBackIMG->GetFrameSize(0).x * 1.2f, 45.f));
	_progressBarBackIMG->FrameRender(_mainRect.left, _mainRect.top, 0, 0, Pivot::LEFT_TOP, false);
	
	//HP바 HP(체력) 이미지
	_progressBarFrontIMG->SetSize(Vector2(_hpWidth *  1.2f, 45.f));
	_progressBarFrontIMG->FrameRender(_mainRect.left, _mainRect.top, _imgFrameX, _imgFrameY, Pivot::LEFT_TOP, false);

	//하트 이미지
	_heartIMG->SetSize(Vector2(48, 48));
	_heartIMG->Render(102, 13, Pivot::LEFT_TOP, false);

}

bool ProgressBar::SetGauge(int currentHp, int maxHp)
{
	//플레이어 클래스 접근해서 플레이어 함수 가져오기
	Player* _player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");

	//만일 saveHp가 currentHp와 다르면
	//FLOAT_EQUAL은 
	if (FLOAT_EQUAL(_saveHp, _player->GetPlayerCurrentHp()) == false)
	{
		//saveHp를 깎는다.
		_saveHp -= 25.0f * _TimeManager->DeltaTime();
		_barState = ProgressState::DamageState;

		if (_saveHp <= _player->GetPlayerCurrentHp())
		{
			_saveHp = _player->GetPlayerCurrentHp();
			_barState = ProgressState::DamageEndState;
		}
			
		//체력 바 계산식 : 체력 바 비율 * 프로그래스 바 이미지 길이만큼
		this->_hpWidth = ((float)_saveHp / (float)maxHp) * (float)_progressBarFrontIMG->GetWidth();

		//체력 바가 0보다 같거나 낮아지면 0으로 초기화
		if (_hpWidth <= 0)
		{
			_hpWidth = 0;
		}
		return true;
	}
	return false;
}

//프로그래스 바 상태
void ProgressBar::ProgressBarState()
{
	switch (_barState)
	{
	//맞은 상태 - 프로그래스 바
	case ProgressBar::ProgressState::DamageState:
		_imgFrameY = 2;
		break;
	//맞은 상태 종료 - 프로그래스 바
	case ProgressBar::ProgressState::DamageEndState:
		//프레임 카운트는 시간 적용
		_frameCount += 35.f * _TimeManager->DeltaTime();
		
		//프레임 카운트가 1초보다 같거나 높아지면
		if (_frameCount >= 1.f)
		{
			//이미지 y 프레임 증가
			_imgFrameY++;
			//프레임 카운트 초기화
			_frameCount = 0.f;

			//프레임 Y가 4보다 커지면
			if (_imgFrameY > 4)
			{
				//프레임 Y는 1로 초기화
				_imgFrameY = 1;
				_barState = ProgressState::DefaultState;
			}
		}
		break;
	//디폴트 상태 - 프로그래스 바
	case ProgressBar::ProgressState::DefaultState:
		_imgFrameY = 1;
		break;
	case ProgressBar::ProgressState::End:

		break;
	default:
		break;
	}
}