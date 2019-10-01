#include "stdafx.h"
#include "Boss.h"
#include "Image.h"
#include "Animation.h"
#include "Player.h"
#include "Rock.h"
#include "Plankton.h"
#include "BossEffect.h"
#include "StringHelper.h"
Boss::Boss()
{
	//보스의 몸통을 생성할 기본적인 골격들
	_name = "Boss";
	this->_imagePosition = Vector2(1070, 340);	//좌표
	this->_size = Vector2(700, 700);		//크기
	this->_pivot = Pivot::CENTER;			//중심위치
	this->_hp = 800;						//체력
	this->_maxHp = 800;						//최대 체력
	this->_speed = 300.0f;					//속도
	this->_damage = 25;						//공격력
	this->_distance = 0.f;					//직선거리 (나중에 넣을거고 코드만짜려고)
	this->_angle = 0.f;						//앵글값 (직선거리와 같음)
	this->_imageRc = Figure::RectMakeCenter(_imagePosition, _imageSize); //렉트생성.
	this->CreateAnimatiom();
	this->_bossCollisionSize = Vector2(335, 300);
	this->_mainRect = Figure::RectMakeCenter(Vector2(_imagePosition.x - 35, _imagePosition.y + 70), _bossCollisionSize);
	//------------1번 공격을 위한 변수들-----------------//
	this->_shadowPosition = Vector2(_imagePosition.x, _imagePosition.y);
	this->_shadowSize = Vector2(50, 50);
	this->_shadowImage = _ImageManager->FindImage("shadow");
	this->_handPosition = Vector2(_shadowPosition.x, _shadowPosition.y - 400);
	this->_handSize = Vector2(150, 150);
	this->_handImage = _ImageManager->FindImage("hand");
	this->_timeCount = 0.f;
	this->_ChasingCount = 0;
	this->_drapCount = 0;
	this->_handFrame = 0;

	this->_shadowScale = 0.f;

	this->_isAttacked = false;			//피격상황 펄스
	this->_isAttackedColor = false;		//초기색 붉은 색
	this->_attackedTimeCount = 0.f;

	CreateHandAnimation();
	//----------------3번 공격을 위한 변수들------------//
	this->_slimePosition = Vector2(_imagePosition.x - 100, _imagePosition.y - 20);
	this->_slimeSize = Vector2(20, 20);
	this->_slimeRc = Figure::RectMakeCenter(_slimePosition, _slimeSize);

	//--------------상태값 초기화------------------//
	//그림자도 아무런 값을 가지면 안되니까 기본상태로 넣어주고
	_shadow = ShadowState::Stop;
	this->ChangeShadowState(ShadowState::Idle);
	//손은 그림자를 x축으로 쫒아가게 둬도안보이니까 체이싱상태로 두고
	_hand = HandState::Up;
	this->ChangeHandState(HandState::Chasing);

	//초기 상태값은 보스가 움직이지 않아야 하니까 가만히 있는 상태를 만들어준다.
	_state = StateType::Create;
	this->ChangeState(StateType::Idle);
}


Boss::~Boss()
{
}

void Boss::Init()
{
	//플레이어의 이름으로 불러와서 내가 쓸수 있게 만들어 준다.
	this->_player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");
}

void Boss::Release()
{
	AniImgIter iter = _aniImgList.begin();
	for (; iter != _aniImgList.end(); ++iter)
	{
		SafeDelete(iter->second->_animation);
		SafeDelete(iter->second);
	}
	_aniImgList.clear();

	_handIter iter1 = _handList.begin();
	for(; iter1 != _handList.end(); ++iter1)
	{
		SafeDelete(iter1->second);
	}
	_handList.clear();
}

void Boss::Update()
{
	//조건별로 처리할 부분은 여기에서
	UpdateState();
	//------------손날리기 스킬----------//
	HandShootHand();
	HandShootShadow();

	AttackedChangeColor();

	////cout << _hp << endl;
	//if (_Input->GetKeyDown('Q'))
	//{
	//	this->AttackedDamage(800);
	//}
	_aniImage->_animation->UpdateFrame();
}

void Boss::Render()
{
	if (_isAttacked == false)
	{
		_aniImage->_bossImage->SetSize(_size);
		_aniImage->_bossImage->FrameRender((int)_imagePosition.x, (int)_imagePosition.y, _aniImage->_animation->GetNowFrameX(), _aniImage->_animation->GetNowFrameY(), _pivot, true);
	}
	if (_isAttacked == true)
	{
		if (_isAttackedColor == true)
		{
			_aniImage->_bossImage_R->SetSize(_size);
			_aniImage->_bossImage_R->FrameRender((int)_imagePosition.x, (int)_imagePosition.y, _aniImage->_animation->GetNowFrameX(), _aniImage->_animation->GetNowFrameY(), _pivot, true);
		}
		if (_isAttackedColor == false)
		{
			_aniImage->_bossImage_W->SetSize(_size);
			_aniImage->_bossImage_W->FrameRender((int)_imagePosition.x, (int)_imagePosition.y, _aniImage->_animation->GetNowFrameX(), _aniImage->_animation->GetNowFrameY(), _pivot, true);
		}
	}

	//죽음 상태가 아닐 때
	if (_state != StateType::Dead)
	{
		//골렘 이름 텍스트 렌더
		_DXRenderer->RenderText(WinSizeX / 2 - 70, WinSizeY / 2 + 200, StringHelper::StringToWString("GOLEM KING"),
			RGB(255, 255, 255), 1.0f, 25.f, DWRITE_TEXT_ALIGNMENT_LEADING, false, L"Cooper Std");

		_shadowScale = 1.4f - (_distance * 0.007f);
		if (_shadowScale < 0.5f)
		{
			_shadowScale = 0.5f;
		}
		if (_shadowScale > 0.8f)
		{
			_shadowScale = 0.8f;
		}

		if (_shadow == ShadowState::Chasing || _shadow == ShadowState::Stop)
		{
			_shadowImage->Render(_shadowPosition.x, _shadowPosition.y, Pivot::CENTER, true);
		}

		if (_handAni != nullptr)
		{
			_handImage->SetSize(_handSize);
			_handImage->FrameRender((int)_handPosition.x, (int)_handPosition.y, _handAni->GetNowFrameX(), _handAni->GetNowFrameY(), Pivot::CENTER, true);
		}

		_shadowImage->SetSize(_shadowImage->GetFrameSize(0));
		_shadowImage->SetAlpha(0.3f);
		_shadowImage->SetScale(_shadowScale);

	}

	if (_isDebug == true)
	{

		_DXRenderer->DrawRectangle(_shadowRc, DefaultBrush::yello, true);
		_DXRenderer->DrawRectangle(_handRc, DefaultBrush::yello, true);
		if (_hand == HandState::Stay)
		{
			_DXRenderer->DrawRectangle(_handRc, DefaultBrush::red, true);
		}
		_DXRenderer->DrawRectangle(_imageRc, DefaultBrush::blue, true);
		_DXRenderer->DrawEllipse(Vector2(_imagePosition.x, _imagePosition.y), _size.x * 0.7f, DefaultBrush::blue, true);
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::blue, true);
		_DXRenderer->DrawRectangle(_slimeRc, DefaultBrush::green, true);
	}

	GameObject::Render();
}


void Boss::ChangeState(StateType state)
{
	//현제 상태와 바꿀 상태가 같다면 이 함수를 빠져나가라.
	if (_state == state)
		return;
	//그게 아니라면 상태를 바꿔라.
	_state = state;
	//상태와 같이 애니메이션도 바꿔줘라.
	this->ChangeAnimation(state);

	switch (_state)
	{
	case Boss::StateType::Idle:
		break;
	case Boss::StateType::Create:
		_SoundManager->Play("bossCreate", 0.5f);
		break;
	case Boss::StateType::Dead:
		//_imagePosition.y = 380.f;
		break;
	case Boss::StateType::Dead2:
		
		break;
	case Boss::StateType::Hand_Shoot_First:
		break;
	case Boss::StateType::Hand_Shoot_Second:
		this->ChangeShadowState(ShadowState::Chasing);
		break;
	case Boss::StateType::Hand_Shoot_Last:
		break;
	case Boss::StateType::Rock_Shoot_First:
		break;
	case Boss::StateType::Rock_Shoot_Second:
		_SoundManager->Play("GolemKingHandCrush", 0.3f);
		RockPattom();
		break;
	case Boss::StateType::Rock_Shoot_Last:
		break;
	case Boss::StateType::Fist_Shoot_First:
		break;
	case Boss::StateType::Fist_Shoot_Second:
		break;
	case Boss::StateType::Fist_Shoot_Last:
		break;
	case Boss::StateType::End:
		break;
	default:
		break;
	}
}

void Boss::UpdateState()
{
	Plankton* _plang;

	switch (_state)
	{
	case Boss::StateType::Idle:
		//플레이어와 보스와의 거리가 보스 사이즈 X * 3 보다 작아진다면 생성으로바꿔라.
		if (_size.x * 0.7f > this->Distance(_imagePosition))
		{
			this->ChangeState(StateType::Create);
		}
		break;
	case Boss::StateType::Create:
		//현재 진행되고있는프레임이 마지막 프레임이라면
		if (_aniImage->_animation->GetNowFrameX() == 31)
		{
			//손날리는 공격의 시작으로 넘겨라.
			ChangeState(StateType::Hand_Shoot_First);

			//보스 : 카메라 흔들기 연출
			_Camera->Shake(9.5f, 1.0f);
		}
		break;
	case Boss::StateType::Dead:
		ChangeHandState(HandState::End);
		ChangeShadowState(ShadowState::End);
		break;
	case Boss::StateType::Hand_Shoot_First:
		//현재 진행되고있는프레임이 마지막 프레임이라면
		if (_aniImage->_animation->GetNowFrameX() == 18)
		{
			//실제 공격의 시작부분으로 넘겨라.
			this->ChangeState(StateType::Hand_Shoot_Second);
		}
		this->Dead();

		break;
	case Boss::StateType::Hand_Shoot_Second:

		this->Dead();
		break;
	case Boss::StateType::Hand_Shoot_Last:

		ChangeHandState(HandState::End);
		ChangeShadowState(ShadowState::End);

		if (_aniImage->_animation->GetNowFrameX() == 10)
		{
			this->ChangeState(StateType::Rock_Shoot_First);
		}

		this->Dead();
		break;
	case Boss::StateType::Rock_Shoot_First:
		if (_aniImage->_animation->GetNowFrameX() == 15)
		{
			_Camera->Shake(3.8f, 0.9f);
			this->ChangeState(StateType::Rock_Shoot_Second);
		}
		this->Dead();
		break;
	case Boss::StateType::Rock_Shoot_Second:
		if (_aniImage->_animation->GetNowFrameX() == 25)
		{
			this->ChangeState(StateType::Rock_Shoot_Last);
		}
		this->Dead();
		break;
	case Boss::StateType::Rock_Shoot_Last:
		if (_aniImage->_animation->GetNowFrameX() == 31)
		{
			this->ChangeState(StateType::Fist_Shoot_First);
		}
		this->Dead();
		break;
	case Boss::StateType::Fist_Shoot_First:
		if (_aniImage->_animation->GetNowFrameX() == 16)
		{
			this->ChangeState(StateType::Fist_Shoot_Second);
		}
		this->Dead();
		break;
	case Boss::StateType::Fist_Shoot_Second:
		_timeCount += _TimeManager->DeltaTime();
		_plang = new Plankton(Vector2(_slimePosition.x , _slimePosition.y + 300));
		_plang->Init();
		
		if (_timeCount > 0.7f)
		{
			_ObjectManager->AddObject(ObjectType::Object, _plang);
			_timeCount = 0.f;
		}

		if (_aniImage->_animation->GetNowFrameX() == 21)
		{
			this->ChangeState(StateType::Fist_Shoot_Last);
		}
		this->Dead();
		break;
	case Boss::StateType::Fist_Shoot_Last:
		if (_aniImage->_animation->GetNowFrameX() == 15)
		{
			this->ChangeState(StateType::Hand_Shoot_First);
		}
		this->Dead();
		break;
	case Boss::StateType::End:
		break;
	default:
		break;
	}
}

void Boss::ChangeAnimation(StateType state)
{
	AniImgIter iter = _aniImgList.find(state);

	if (iter != _aniImgList.end())
	{
		_aniImage = iter->second;
		_aniImage->_animation->Stop();
		_aniImage->_animation->Play();
	}
}

void Boss::CreateAnimatiom()
{
	
	//플레이어가 에너미의 인지반경 밖에 있을경우 실행되는 애니메이션과 이미지
	AniAndImage* idle = new AniAndImage;
	idle->_animation = new Animation;
	idle->_bossImage = _ImageManager->FindImage("create");
	idle->_bossImage_R = _ImageManager->FindImage("create_Red");
	idle->_bossImage_W = _ImageManager->FindImage("create_White");
	idle->_animation->SetStartEndFrame(0, 0, 0, 0, false);
	idle->_animation->SetIsLoop(false);
	idle->_animation->SetFrameUpdateTime(1.f);
	_aniImgList.insert(make_pair(StateType::Idle, idle));

	//플레이어를 에너미가 인식하고 공격모션이 나오기 전에 자기 자신을 꺠우는 애니&이미지
	AniAndImage* create = new AniAndImage;
	create->_animation = new Animation;
	create->_bossImage = _ImageManager->FindImage("create");
	create->_bossImage_R = _ImageManager->FindImage("create_Red");
	create->_bossImage_W = _ImageManager->FindImage("create_White");
	create->_animation->SetStartEndFrame(0, 0, 31, 0, false);
	create->_animation->SetIsLoop(false);
	create->_animation->SetFrameUpdateTime(0.1f);
	_aniImgList.insert(make_pair(StateType::Create, create));

	//보스가 죽을때 나오는 애니&이미지
	AniAndImage* dead1 = new AniAndImage;
	dead1->_animation = new Animation;
	dead1->_bossImage = _ImageManager->FindImage("dead1");
	dead1->_bossImage_R = _ImageManager->FindImage("dead1_Red");
	dead1->_bossImage_W = _ImageManager->FindImage("dead1_White");
	dead1->_animation->SetStartEndFrame(0, 0, 40, 0, false);
	dead1->_animation->SetIsLoop(false);
	dead1->_animation->SetFrameUpdateTime(0.1f);
	dead1->_animation->SetCallbackFunc([this]() {this->NextAnimation(); });
	_aniImgList.insert(make_pair(StateType::Dead, dead1));

	//보스가 죽을때 나오는 애니& 이미지2
	AniAndImage* dead2 = new AniAndImage;
	dead2->_animation = new Animation;
	dead2->_bossImage = _ImageManager->FindImage("dead2");
	dead2->_bossImage_R = _ImageManager->FindImage("dead2_Red");
	dead2->_bossImage_W = _ImageManager->FindImage("dead2_White");
	dead2->_animation->SetStartEndFrame(0, 0, 40, 0, false);
	dead2->_animation->SetIsLoop(false);
	dead2->_animation->SetFrameUpdateTime(0.1f);
	_aniImgList.insert(make_pair(StateType::Dead2, dead2));

	//보스의 1번째 공격패턴 손날려서 공격하기의 전조 손을 날리는 애니&이미지
	AniAndImage* hand_Shoot_First = new AniAndImage;
	hand_Shoot_First->_animation = new Animation;
	hand_Shoot_First->_bossImage = _ImageManager->FindImage("hand_Shoot_First");
	hand_Shoot_First->_bossImage_R = _ImageManager->FindImage("hand_Shoot_First_Red");
	hand_Shoot_First->_bossImage_W = _ImageManager->FindImage("hand_Shoot_First_White");
	hand_Shoot_First->_animation->SetStartEndFrame(0, 0, 19, 0, false);
	hand_Shoot_First->_animation->SetIsLoop(false);
	hand_Shoot_First->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Hand_Shoot_First, hand_Shoot_First));

	//보스의 1번째 공격패턴 손날려서 공격하기의 손은 이미 날아가고 난뒤의 몸의 애니&이미지
	AniAndImage* hand_Shoot_Second = new AniAndImage;
	hand_Shoot_Second->_animation = new Animation;
	hand_Shoot_Second->_bossImage = _ImageManager->FindImage("hand_Shoot_second");
	hand_Shoot_Second->_bossImage_R = _ImageManager->FindImage("hand_Shoot_second_Red");
	hand_Shoot_Second->_bossImage_W = _ImageManager->FindImage("hand_Shoot_second_White");
	hand_Shoot_Second->_animation->SetStartEndFrame(0, 0, 29, 0, false);
	hand_Shoot_Second->_animation->SetIsLoop(true);
	hand_Shoot_Second->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Hand_Shoot_Second, hand_Shoot_Second));

	//보스의 1번째 공격패턴 손날려서 공격하기의 손이 돌아오는 애니&이미지
	AniAndImage* hand_Shoot_Last = new AniAndImage;
	hand_Shoot_Last->_animation = new Animation;
	hand_Shoot_Last->_bossImage = _ImageManager->FindImage("hand_Shoot_Last");
	hand_Shoot_Last->_bossImage_R = _ImageManager->FindImage("hand_Shoot_Last_Red");
	hand_Shoot_Last->_bossImage_W = _ImageManager->FindImage("hand_Shoot_Last_White");
	hand_Shoot_Last->_animation->SetStartEndFrame(0, 0, 10, 0, false);
	hand_Shoot_Last->_animation->SetIsLoop(false);
	hand_Shoot_Last->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Hand_Shoot_Last, hand_Shoot_Last));

	//보스의 2번째 스킬 땅을 내려찍는부분.
	AniAndImage* Rock_Shoot_First = new AniAndImage;
	Rock_Shoot_First->_animation = new Animation;
	Rock_Shoot_First->_bossImage = _ImageManager->FindImage("Rock_Shoot");
	Rock_Shoot_First->_bossImage_R = _ImageManager->FindImage("Rock_Shoot_Red");
	Rock_Shoot_First->_bossImage_W = _ImageManager->FindImage("Rock_Shoot_White");
	Rock_Shoot_First->_animation->SetStartEndFrame(0, 0, 15, 0, false);
	Rock_Shoot_First->_animation->SetIsLoop(false);
	Rock_Shoot_First->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Rock_Shoot_First, Rock_Shoot_First));
	//보스의 2번째 스킬 돌이떨어지는걸 기다리면서 대기하는부분.
	AniAndImage* Rock_Shoot_Second = new AniAndImage;
	Rock_Shoot_Second->_animation = new Animation;
	Rock_Shoot_Second->_bossImage = _ImageManager->FindImage("Rock_Shoot");
	Rock_Shoot_Second->_bossImage_R = _ImageManager->FindImage("Rock_Shoot_Red");
	Rock_Shoot_Second->_bossImage_W = _ImageManager->FindImage("Rock_Shoot_White");
	Rock_Shoot_Second->_animation->SetStartEndFrame(15, 0, 25, 0, false);
	Rock_Shoot_Second->_animation->SetIsLoop(false);
	Rock_Shoot_Second->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Rock_Shoot_Second, Rock_Shoot_Second));
	//보스의 2번째 스킬 돌이 사라지고 기본상태로 돌아오는 부분.
	AniAndImage* Rock_Shoot_Last = new AniAndImage;
	Rock_Shoot_Last->_animation = new Animation;
	Rock_Shoot_Last->_bossImage = _ImageManager->FindImage("Rock_Shoot");
	Rock_Shoot_Last->_bossImage_R = _ImageManager->FindImage("Rock_Shoot_Red");
	Rock_Shoot_Last->_bossImage_W = _ImageManager->FindImage("Rock_Shoot_White");
	Rock_Shoot_Last->_animation->SetStartEndFrame(25, 0, 31, 0, false);
	Rock_Shoot_Last->_animation->SetIsLoop(false);
	Rock_Shoot_Last->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Rock_Shoot_Last, Rock_Shoot_Last));

	//보스의 3번째 스킬 플레이어를 공격하기 전의 준비상태.
	AniAndImage* Fist_Shoot_First = new AniAndImage;
	Fist_Shoot_First->_animation = new Animation;
	Fist_Shoot_First->_bossImage = _ImageManager->FindImage("FistShoot");
	Fist_Shoot_First->_bossImage_R = _ImageManager->FindImage("FistShoot_Red");
	Fist_Shoot_First->_bossImage_W = _ImageManager->FindImage("FistShoot_White");
	Fist_Shoot_First->_animation->SetStartEndFrame(0, 0, 16, 0, false);
	Fist_Shoot_First->_animation->SetIsLoop(false);
	Fist_Shoot_First->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Fist_Shoot_First, Fist_Shoot_First));
	//보스가 플레이어를 향해 팔을 조준하고 있는 상태.
	AniAndImage* Fist_Shoot_Second = new AniAndImage;
	Fist_Shoot_Second->_animation = new Animation;
	Fist_Shoot_Second->_bossImage = _ImageManager->FindImage("FistShoot");
	Fist_Shoot_Second->_bossImage_R = _ImageManager->FindImage("FistShoot_Red");
	Fist_Shoot_Second->_bossImage_W = _ImageManager->FindImage("FistShoot_White");
	Fist_Shoot_Second->_animation->SetStartEndFrame(0, 1, 21, 1, false);
	Fist_Shoot_Second->_animation->SetIsLoop(false);
	Fist_Shoot_Second->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Fist_Shoot_Second, Fist_Shoot_Second));
	//보스가 플레이어를 향해 팔을 쏘고 휘두르며 원래대로 돌아오게 하는 상태.
	AniAndImage* Fist_Shoot_Last = new AniAndImage;
	Fist_Shoot_Last->_animation = new Animation;
	Fist_Shoot_Last->_bossImage = _ImageManager->FindImage("FistShoot");
	Fist_Shoot_Last->_bossImage_R = _ImageManager->FindImage("FistShoot_Red");
	Fist_Shoot_Last->_bossImage_W = _ImageManager->FindImage("FistShoot_White");
	Fist_Shoot_Last->_animation->SetStartEndFrame(0, 2, 15, 2, false);
	Fist_Shoot_Last->_animation->SetIsLoop(false);
	Fist_Shoot_Last->_animation->SetFrameUpdateTime(0.15f);
	_aniImgList.insert(make_pair(StateType::Fist_Shoot_Last, Fist_Shoot_Last));
}
//보스가 죽는이미지가 커서 쪼갬.
void Boss::NextAnimation()
{
	if (_state == StateType::Dead)
	{
		ChangeAnimation(StateType::Dead2);
		_imagePosition.y = 435.f;
	}

}

void Boss::RockPattom()
{
	for (int j = 0; j < 5; ++j)
	{
		//돌떨구는 공식.
		float timeCount = 1.f + (0.2f * j);

		for (int i = 0; i < 7; ++i)
		{
			float x = (_imagePosition.x) + cosf(Math::PI + (Math::PI / 6) * i) * (300 + (100 * j));
			float y = (_imagePosition.y - 1000) + (-sinf(Math::PI + (Math::PI / 6) * i)) * (300 + (100 * j));
			_ObjectManager->AddObject(ObjectType::Object, new Rock(Vector2(x, y), timeCount));
		}
	}

}
//피격시 이미지
void Boss::AttackedDamage(int damage)
{
	_hp -= damage;
	if (_hp <= 0)
	{
		_SoundManager->Play("bossDeath", 0.7f);

		ChangeState(StateType::Dead);
	}
	else
	{
		_SoundManager->Play("golemHit", 0.5f);
		_Camera->Shake(0.6f, 0.8f);
		_isAttacked = true;
	}
}
//죽을때 불러와질 함수.
void Boss::Dead()
{
	//스킬을쓰는 도중에 체력이 0이나 0이하라면
	if (_hp <= 0)
	{
		_Camera->Shake(10.0f, 7.0f);
		//죽는상태로 넘겨라.
		this->ChangeState(StateType::Dead);
	}
}

void Boss::AttackedChangeColor()
{
	if (_isAttacked == true)
	{
		_attackedTimeCount += _TimeManager->DeltaTime();
		if (_attackedTimeCount <= 0.2f)
		{
			if (_attackedTimeCount <= 0.1f)
			{
				_isAttackedColor = false;
			}
			if (_attackedTimeCount > 0.1f)
			{
				_isAttackedColor = true;
			}
		}
		else if (_attackedTimeCount > 0.2f)
		{
			_attackedTimeCount = 0.0f;
			_isAttacked = false;
		}
	}

	if (_isAttackedColor == false)
	{

	}
	if (_isAttackedColor == true)
	{

	}
}

//직선거리 길이 구하는 공식.
float Boss::Distance(Vector2 position)
{
	//직선거리를 구하고
	float distance = Math::GetDistance(position.x, position.y, _player->GetPosition().x, _player->GetPosition().y);
	//반환해
	return distance;
}

//앵글값구하는 공식.
float Boss::Angle(Vector2 position)
{
	//앵글값을 구하고
	float angle = Math::GetAngle(position.x, position.y, _player->GetPosition().x, _player->GetPosition().y);
	//반환해
	return angle;
}
