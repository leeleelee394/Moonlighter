#pragma once
#include "GameObject.h"

enum class ShadowState
{
	Chasing, Stop, Idle, End
};

enum class HandState
{
	 Up, Down, Stay, Chasing, End
};



class Boss : public GameObject
{
private:
	//보스의 상태 종류 , 플레이어를 마주치기 전, 마주치고나서  깨어날때, 죽을때, 손떨구기, 주먹발사, 돌떨구기., 
	enum class StateType
	{
		Idle, Create,Dead,Dead2, 
		Hand_Shoot_First, Hand_Shoot_Second, Hand_Shoot_Last,
		Rock_Shoot_First, Rock_Shoot_Second, Rock_Shoot_Last,
		Fist_Shoot_First, Fist_Shoot_Second, Fist_Shoot_Last, 
		End
	};

	struct AniAndImage
	{
		class Image* _bossImage;
		class Image* _bossImage_R;
		class Image* _bossImage_W;
		class Animation* _animation;
	};
private:
	//플레이어를 불러오자.
	class Player* _player;
	class Plankton* _plankton;
	//상태를 불러오기위해서 이넘값도 가져오자.
	StateType _state;
	AniAndImage* _aniImage;

	typedef map<StateType, AniAndImage*>::iterator AniImgIter;
	map<StateType, AniAndImage*> _aniImgList;

//이미지
#pragma region Image
	class Image* _shadowImage;		//그림자 이미지

	class Image* _handImage;		//손 날리기 스킬의 손 이미지
										 
	class Image* _rockImgae1;		//돌 던지기 스킬의 돌던지기 1
	class Image* _rockImage2;		//돌 던지기 스킬의 돌던지기 2
	class Image* _rockImage3;		//돌 던지기 스킬의 돌던지기 3
	
	class Image* _slimeArmImage1;	//손 던지기 스킬의 슬라임 팔 1
	class Image* _slimeArmImage2;	//손 던지기 스킬의 슬라임 팔 2
#pragma endregion	

	int _hp;					//체력이 있어야 할 거고
	int _maxHp;					//보스 맥스 HP
	int _damage;				//플레이어를 공격해야하니까 데미지도 있을 것이고,
	float _speed;				//각 공격들은 속도를 가져야하니까 속도도 들어가겠지 뭐
	float _mainSpeed;
	float _angle;				//각도를 계산해야하니까 앵글값도 있을 것이고,
	float _distance;			//플레이어가 일정거리 안에 들어서면 움직여야하니까 플레이어의 거리를 재기 위해서 디스턴스도 있어야 하고,
	int _handFrame;

	float _shadowScale;
	Vector2 _bossCollisionSize;	//충돌렉트 사이즈

	bool _isAttacked;		//false = Not Attacked, true = Attacked;	
	bool _isAttackedColor;	//false = Red, true = White;
	float _attackedTimeCount;
	//-----------------보스 이미지 생성을 위한 렉트-----------------//
	Vector2 _imagePosition;
	Vector2 _imageSize;
	RECT _imageRc;

	//-------충돌을 위한 렉트-------//
	RECT collsionRc;

	//----------------손의 렉트--------------//
	RECT _rockHandRc;				//오른손주먹도 맞으면 아프니까 렉트가있어야 하고,

	//-----------------돌의 렉트--------------//
	Vector2 _rockPosition;		//돌들마다 충돌이 되어야 하고 각자의 좌표를 뿌려줘야하니까 얘들의 좌표를 주기위해서 좌표가있어야겠지?
	Vector2 _rockSize;			//돌들도 크기가 있을거 아녀 그럼 사이즈가잇어야겠지
	RECT _rockRc;				//좌표 있고 크기 있으면 그려줘야지

	//----------------주먹의 렉트-------------//
	Vector2 _fistPosition;		//플레이어의 렉트와 내 슬라임의 쬰쬬니가 충돌시켜야하니까 쬰쬬니의 선을 그려주고, 
	Vector2 _fistSize;			//쬰쪼니에 달린 주먹 크기도 있어야 할거 아녀
	RECT _fistRc;				//쬰쬬니에 달린 주먹의 렉트도 있어야 겠지?
	RECT _attackedRc;			//공격을 맞기 위한 렉트가 있어야 하고,
	
	//보스의 슬라임의 중심좌표.
	Vector2 _slimePosition;
	Vector2 _slimeSize;
	RECT _slimeRc;

	//---------------손공격을 위한 렉트-------//
	Vector2 _handPosition;
	Vector2 _handSize;
	RECT _handRc;

	//------플레이어를 추격하기 위한 그림자 렉트----//
	Vector2 _shadowPosition;		//그림자 좌표
	Vector2 _shadowSize;			//그림자 크기
	RECT _shadowRc;					//그림자 렉트

	HandState _hand;
	Animation* _handAni;
	typedef map <HandState, Animation*>::iterator _handIter;
	map<HandState, Animation*> _handList;

	ShadowState _shadow;
	
	float _timeCount;				//델타타임과 동기화 하기 위한 카운트
	int _ChasingCount;
	int _drapCount;
	
	class Rock* _rock;
	

	//1스킬의 First가 끝나면 Second로 전환
	//Second로 넘어가면 바로 그림자가 player를 추격
	//그림자의 RECT가 플레이어와 충돌되는동안 Count를 추가하고,
	//일정 카운트가 어느정도 진행된다면,
	//그림자의 RECT를 정지시키고 팔을 불러오기 위한 불변수를 True로 바꾸어
	//맵의 밖에서 X축 추적을 계속 하고 있던 팔의 X축 Speed를 멈추고 Y축의 속도를
	//줘서 빠르게 낙하시킨다.

	//그림자의 position과 팔의 positiom이 같다면 1초의 Count가 지난 후 다시
	//올라가게 하고서 횟수 카운트를 1 추가시킨다.
	//5의 횟수카운트가 채워진다면 카운트를 0으로 바꿔주고 Last상태로 돌입한다.

public:
	virtual void AttackedDamage(int damage);
	float Distance(Vector2 position);
	float Angle(Vector2 position);
	
	void ChangeState(StateType state);
	void UpdateState();
	void ChangeAnimation(StateType state);
	void CreateAnimatiom();
	void Dead();
	void AttackedChangeColor();
	//-------------------------------------------------------//
	//--------------1번스킬 HandShoot 함수들------------------//
	//-------------------------------------------------------//
	void HandShootShadow();
	void HandShootHand();
	void ChangeHandState(HandState hand);
	void ChangeHandAnimation(HandState hand);
	void CreateHandAnimation();
	void ChangeShadowState(ShadowState shadow);
	void NextAnimation();
	//-------------------------------------------------------//
	//--------------2번스킬 RockShoot 함수들------------------//
	//-------------------------------------------------------//
	void RockPattom();

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	//보스 현재 체력 GET함수
	int GetBossCurrentHp() { return _hp; }

	//보스 최대 체력 GET함수
	int GetBossMaxHp() { return _maxHp; }

public:
	Boss();
	~Boss();
};