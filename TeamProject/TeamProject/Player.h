#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	enum class State : int
	{
		LeftIdle = 0, RightIdle, UpIdle, DownIdle,
		LeftRun, RightRun, UpRun, DownRun,
		LeftRoll, RightRoll, UpRoll, DownRoll,
		LeftSword1, RightSword1, UpSword1, DownSword1,
		LeftSword2, RightSword2, UpSword2, DownSword2,
		LeftBow,RightBow,UpBow,DownBow,
		Die,
		end
	};

private:
	//애니메이션을 담을 맵의 반복자
	typedef map<State, class Animation*>::iterator AnimationIter;
private:
	class Image* _imgMove;		//플레이어 기본 움직임
	class Image* _imgAtkSword;	//플레이어 공격 스워드 이미지
	class Image* _imgAtkBow;	//플레이어 공격 활 이미지
	class Image* _imgShadow;

	class UIWeaponAndBag* _UIWeaponSwap;
	class Arrow* _arrow;
	class Animation* _mainAnimation;
	
	//vector <Arrow> _arrow;//화살 클래스 사용을 위한 선언


	map<State, class Animation*> _animationList;	//상태별 애니를 관리하기 위해 맵 사용
	State _state;
	
	RECT _collisionRect;	//충돌용 RECT
	RECT _swordRect;		//칼 공격용 RECT
	RECT _shieldRect;		//방어용 RECT
	
	float _speed;
	
	int _currentHp;
	int _maxHp;
	int _damage;

	float _frameRun;
	float _frameIdle;
	float _frameBow;

	bool _isMoveStop;
	bool _isChangeSword;		//칼 이미지와 스탠드 무브 전환용 bool값
	bool _isChangeBow;
	bool _isStandardMove;

	int _swordWidth;
	int _swordHeight;
	
	bool _isAttacked;
	bool _isChangeWeapon;
	bool _isTwice;
	bool _isDam;
	int _alpha;

	bool _isDelay;
	float _count;
	int _blink;


public:
	Player(Vector2 pos);
	~Player();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

public:
	void Move(Vector2 direction);
	void ChangeAnimation(State state);
	void ChangeState(State state);
	void CreateAnimation();	
	bool InterRee(RECT* moveRc, RECT* unMoveRc);

	void AttackedDamage(int damage);
	void InventoryOnOff();

	void EndAnimation();
	void Attack();
	void AtkDelay2();
	
	POINT GetPlayerIndex();
	int GetPlayerCurrentHp() { return _currentHp; }
	int GetPlayerMaxHp() { return _maxHp; }
	RECT GetCollisionRect()const override { return _collisionRect; }
	RECT GetSwordRect() const { return _swordRect; }
	int GetPlayerDamage() { return _damage; }		//에너미 클래스에 넘겨주기 위한 함수
	State GetState() { return _state; }				//동작 enum값 전달 함수
};

class PlayerAni
{
public:
	PlayerAni();
	~PlayerAni();
};
