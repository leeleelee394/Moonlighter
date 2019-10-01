#pragma once
#include "GameObject.h"
class TestPlayer : public GameObject
{
	//상태 열거자
	enum class State : int
	{
		LeftIdle = 0 ,RightIdle,UpIdle,DownIdle, 
		LeftRun,RightRun,UpRun,DownRun
	};
	struct TestBullet
	{
		Vector2 pos; 
		Vector2 direction; 
		float speed; 
		bool bFire; 
	};

private:
	//애니메이션 담을 맵의 반복자 
	typedef map<State, class Animation*>::iterator AnimationIter;
private:
	//플레이어 이미지 
	class Image* _image;
	//애니메이션 리스트
	//상태별로 애니메이션을 관리하기 위해서 맵을 사용
	map<State, class Animation*> _animationList;

	State _state;						//상태
	class Animation* _mainAnimation;	//메인으로 돌아갈 애니메이션 
	RECT _collisionRect;				//정밀 충돌용 렉트
	float _speed;						//이동속도
public:
	TestPlayer();
	~TestPlayer();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
	
	virtual RECT GetCollisionRect()const override {return  _collisionRect; }
private:
	void Move(Vector2 direction);
	void ChangeAnimation(State state);
	void ChangeState(State state);
	void CreateAnimation();

	void IdleKeyInput();
};

