#include "stdafx.h"
#include "BossProgressBar.h"
#include "Boss.h"


BossProgressBar::BossProgressBar()
{
	//보스 프로그래스 바 이미지
	_bossProgressBarImg = _ImageManager->FindImage("bossHpBar");

	//클래스 키 이름 선언
	this->_name = "bossProgressBar";

	//포지션 초기화
	this->_position = Vector2(100, 600);
	//렉트 생성
	this->_mainRect = Figure::RectMake(_position.x, _position.y, _bossProgressBarImg->GetWidth(), _bossProgressBarImg->GetHeight());

	//이미지 프레임 초기화 
	_imgFrameX = 0;
	_imgFrameY = 1;

	//세이브 hp 는 currentHp;
	Boss* _boss = (Boss*)_ObjectManager->FindObject(ObjectType::Object, "Boss");
	_saveHp = _boss->GetBossMaxHp();

	//디폴트 상태
	_bossProgressState = BossProgressState::DefaultState;

	SetGauge(_boss->GetBossCurrentHp(), _boss->GetBossMaxHp());
	this->_hpWidth = ((float)_saveHp / (float)_boss->GetBossMaxHp()) * (float)_bossProgressBarImg->GetWidth();
}


BossProgressBar::~BossProgressBar()
{
}

void BossProgressBar::Init()
{

}

void BossProgressBar::Release()
{

}

void BossProgressBar::Update()
{
	//메인 렉트 선언
	this->_mainRect = Figure::RectMake(_position.x, _position.y, _bossProgressBarImg->GetWidth(), _bossProgressBarImg->GetHeight());

	//플레이어 클래스 접근해서 플레이어 함수 가져오기
	Boss* _boss = (Boss*)_ObjectManager->FindObject(ObjectType::Object, "Boss");


	//변화한 HP바 길이 얻기
	SetGauge(_boss->GetBossCurrentHp(), _boss->GetBossMaxHp());

	//프로그래스 바 상태 함수
	BossProgressBarState();

	//업데이트 렉트
	this->UpdateMainRect();
}

void BossProgressBar::Render()
{
	//보스 프로그래스 바 이미지 크기
	_bossProgressBarImg->SetSize(Vector2(_hpWidth * 1.6f, 45.f));

	//보스 프로그래스 바 알파 값
	_bossProgressBarImg->SetAlpha(0.7f);

	//보스 프로그래스바 바 프레임 렌더
	_bossProgressBarImg->FrameRender(_mainRect.left, _mainRect.top, _imgFrameX, _imgFrameY, Pivot::LEFT_TOP, false);
}

bool BossProgressBar::SetGauge(int currentHp, int maxHp)
{
	//플레이어 클래스 접근해서 플레이어 함수 가져오기
	Boss* _boss = (Boss*)_ObjectManager->FindObject(ObjectType::Object, "Boss");

	//만일 saveHp가 currentHp와 다르면
	//FLOAT_EQUAL은 
	if (FLOAT_EQUAL(_saveHp, _boss->GetBossCurrentHp()) == false)
	{
		//saveHp를 깎는다.
		_saveHp -= 60.0f * _TimeManager->DeltaTime();
		_bossProgressState = BossProgressState::DamageState;

		if (_saveHp <= _boss->GetBossCurrentHp())
		{
			_saveHp = _boss->GetBossCurrentHp();
			_bossProgressState = BossProgressState::DamageEndState;
		}

		//체력 바 계산식 : 체력 바 비율 * 프로그래스 바 이미지 길이만큼
		this->_hpWidth = ((float)_saveHp / (float)maxHp) * (float)_bossProgressBarImg->GetWidth();

		//체력 바가 0보다 같거나 낮아지면 0으로 초기화
		if (_hpWidth <= 0)
		{
			_hpWidth = 0;
		}
		return true;
	}
	return false;
}

void BossProgressBar::BossProgressBarState()
{
	switch (_bossProgressState)
	{
		//맞은 상태 - 프로그래스 바
	case BossProgressBar::BossProgressState::DamageState:
		_imgFrameY = 1;
		break;
		//맞은 상태 종료 - 프로그래스 바
	case BossProgressBar::BossProgressState::DamageEndState:
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
				//프레임 Y는 0로 초기화
				_imgFrameY = 0;
				_bossProgressState = BossProgressState::DefaultState;
			}
		}
		break;
		//디폴트 상태 - 프로그래스 바
	case BossProgressBar::BossProgressState::DefaultState:
		_imgFrameY = 0;
		break;
	case BossProgressBar::BossProgressState::End:

		break;
	default:
		break;
	}
}
