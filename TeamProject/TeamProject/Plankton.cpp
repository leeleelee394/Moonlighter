#include "stdafx.h"
#include "Plankton.h"
#include "Player.h"
#include "MoveItem.h"
#include "Animation.h"


Plankton::Plankton(Vector2 position)
{
	//클래스 이름 (오브젝트 키)
	this->_name = "plankton";

	//프레임 이미지 추가
	this->_plankton = _ImageManager->FindImage("planktonIMG");
	this->_planktonRed = _ImageManager->FindImage("planktonRedIMG");
	this->_planktonWhite = _ImageManager->FindImage("planktonWhiteIMG");

	//애니메이션 new
	_planktonAnimation = new Animation;

	//시작 프레임 x,y와 끝 프레임 x,y
	_planktonAnimation->SetStartEndFrame(0, 0, _plankton->GetMaxFrameX() - 1, 0, false);
	//루프 안함
	_planktonAnimation->SetIsLoop(true);
	//프레임 도는 시간
	_planktonAnimation->SetFrameUpdateTime(0.2f);
	//프레임 시작
	_planktonAnimation->Play();

	//포지션
	this->_position = position;
	//데미지
	this->_damage = 5;
	//HP	
	this->_hp = 75;
	//스피드
	this->_speed = 35.0f;
	//사이즈
	this->_size = Vector2(30, 30);
	//피벗
	this->_pivot = Pivot::CENTER;
	//렉트 생성
	this->_renderRect = UpdateRect(_position, _size, _pivot);

	//기본 상태는 따라가기
	this->_planktonState = PlanktonState::Follow;

	//공격 bool
	_isAttack = false;

	//인벤 정지 bool
	_isInvenStop = false;

	//피격 bool
	_isAtked = false;

	//공격 카운트 초기화
	_atkCount = 0;

	//델타 카운트 초기화
	deltaTimeCount = 0.f;

	//피격 카운트 초기화
	atkedCount = 0.f;

	//받은 메시지 활성화
	this->AddCallbackMessage("InventoryOpen", [this](TagMessage message) {this->InvenStop(1); });
	this->AddCallbackMessage("InventoryClose", [this](TagMessage message) {this->InvenStop(0); });
}


Plankton::~Plankton()
{
}

void Plankton::Init()
{
	//에너미 Init
	Enemy::Init();
}

void Plankton::Release()
{
	//릴리즈
	SafeDelete(_planktonAnimation);
}

void Plankton::Update()
{
	_renderRect = UpdateRect(_position, _size, _pivot);

	//공격 렉트 생성
	_atkRect = UpdateRect(_position, _size, Pivot::CENTER);

	//플랑크톤 렉트와 플레이어 렉트가 충돌하면 상태를 공격 상태로 변경
	RECT temp;
	if (IntersectRect(&temp, &_renderRect, &_player->GetCollisionRect()))
	{
		//공격 상태 변경
		_planktonState = PlanktonState::Attack;
	}

	if (_planktonState != PlanktonState::Attack) 
	{
		//피격 상태 true
		if (_attacked == true)
		{
			//피격 상태 변경
			_planktonState = PlanktonState::Attacked;
		}
		else
		{
			_planktonState = PlanktonState::Follow;
		}
	}

	//오브젝트 충돌 체크 함수
	this->ObjectCollision();

	if (!_isInvenStop)
	{
		//플랑크톤 상태 함수
		PlanktonStates();
	}

	//플랑크톤 프레임 업데이트
	_planktonAnimation->UpdateFrame();
}

void Plankton::Render()
{
	//만일 피격 상태인 경우
	if (_planktonState == PlanktonState::Attacked) 
	{
		//플랑크톤 사이즈
		//플랑크톤 빨강
		_planktonRed->SetSize(_plankton->GetFrameSize(0));
		//플랑크톤 하양
		_planktonWhite->SetSize(_plankton->GetFrameSize(0));

		//만일 피격 렌더가 true이면
		if (_isAtkedRender == true) 
		{
			//플랑크톤 프레임 렌더
			_planktonRed->FrameRender(_position.x, _position.y, _planktonAnimation->GetNowFrameX(), 0, Pivot::CENTER, true);
		}
		//만일 피격 렌더가 false이면
		if (_isAtkedRender == false) 
		{
			//플랑크톤 프레임 렌더
			_planktonWhite->FrameRender(_position.x, _position.y, _planktonAnimation->GetNowFrameX(), 0, Pivot::CENTER, true);
		}
	}

	//만일 따라가기 || 공격 상태인 경우
	if (_planktonState == PlanktonState::Follow || _planktonState == PlanktonState::Attack)
	{
		//플랑크톤 사이즈
		_plankton->SetSize(_plankton->GetFrameSize(0));
		//플랑크톤 프레임 렌더
		_plankton->FrameRender(_position.x, _position.y, _planktonAnimation->GetNowFrameX(), 0, Pivot::CENTER, true);
	}

	//디버그 (F1)
	if (_isDebug) 
	{
		//빨간 사각형 렌더
		_DXRenderer->DrawRectangle(_renderRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(_atkRect, DefaultBrush::gray, true, 5.0f);
	}

}

//플랑크톤 상태 함수 (switch문)
void Plankton::PlanktonStates()
{
	switch (_planktonState)
	{
	//상태 : 따라가기 Follow
	case Plankton::PlanktonState::Follow:
		//앵글 값 구하기
		this->_angle = Math::GetAngle(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);
		//구한 앵글 값으로 플레이어한테 이동시키기
		this->_position.x += cosf(_angle) * _speed * _TimeManager->DeltaTime();
		this->_position.y -= sinf(_angle) * _speed * _TimeManager->DeltaTime();
		//렉터 업데이트
		this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);

		break;

	//상태 : 공격 Attack
	case Plankton::PlanktonState::Attack:

		RECT temp;
		//공격 렉트와 플레이어 렉트가 충돌하면
		if (IntersectRect(&temp, &_atkRect, &_player->GetCollisionRect()))
		{
			//피격 : 플레이어 데미지 전달
			_player->AttackedDamage(_damage);

			//따라가기 상태로 변경
			_planktonState = PlanktonState::Follow;
		}
		break;

	//상태 : 피격 Attacked
	case Plankton::PlanktonState::Attacked:
		//피격 상태인 경우, 밀리기와 컬러 렌더 시간 조절용 함수
		this->AtkedState(_position, _attackedAngle, 100.0f, 0.2f);
		break;

	//상태 : 죽음 Dead
	case Plankton::PlanktonState::Dead:

		break;
	case Plankton::PlanktonState::End:
		break;
	default:
		break;
	}
}


//피격 상태인 경우, 밀리기와 컬러 렌더 시간 조절용 함수
void Plankton::AtkedState(Vector2 position, float angle, float speed, float countValue)
{
	//델타 타임++
	deltaTimeCount += _TimeManager->DeltaTime();

	//피격 카운트++
	atkedCount += _TimeManager->DeltaTime();

	//델타 타임 0.5초보다 작거나 같아지면
	if (deltaTimeCount <= 0.5f) 
	{
		//포지션과 렉트 조정
		_position.x += cosf(angle) * speed * _TimeManager->DeltaTime();
		_position.y -= sinf(angle) * speed * _TimeManager->DeltaTime();
		this->_renderRect = UpdateRect(_position, _size, Pivot::CENTER);

		//피격 카운트에 따라 피격 렌더 설정하기
		if (atkedCount < countValue)
		{
			//피격 렌더 (빨강색)
			_isAtkedRender = true;
		}
		if (atkedCount > countValue)
		{
			//피격 렌더 (흰색)
			_isAtkedRender = false;
		}
	}
	//델타 타임 0.5초보다 커지면
	else if (deltaTimeCount > 0.5f) 
	{
		//델타 타임 초기화
		deltaTimeCount = 0;

		//피격 카운트 값 초기화
		atkedCount = 0;

		//피격 상태 false
		_attacked = false;

	}
}

void Plankton::InvenStop(bool isInvenStop)
{
	//함수가 호출되면 _isInvenStop의 상태를 인자 값으로 변경 
	_isInvenStop = isInvenStop;
}
