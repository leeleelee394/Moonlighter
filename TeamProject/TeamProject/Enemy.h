#pragma once
#include "GameObject.h"
class Enemy : public GameObject
{
protected:
	//움직임의 타입. 방향을 알기 위해서 만든 이넘값
	enum class MoveType
	{
		Top = 0, Left, Right, Bottom, End
	};

	//쫒는상태와 공격 상태를 구분하기 위해서 만들어 놓은 이넘값.
	//방향과 상태를 구분하기 위해서 만들었음. 피격시 이미지가 바뀌거나 하지 않고 움직이는 방향이 뒤틀리기도 않으므로 
	//굳이 Attacked를 만들 필요가 없고 문라이트의 에너미의 경우에는 기본 대기상태 자체가 없다고 봐도 무방하으로 Idle상태로 없음.
	enum class StateType
	{
		Chasing = 0, attack
	};

protected:
	float _speed;			//속도
	float _distance;		//직선거리를 구하기 위한 변수
	float _angle;			//앵글
	float _attackedAngle;	//공격을 받았을 시에 튕겨져 나갈 방향.
	int _damage;			//공격력
	int _hp;				//체력
	RECT _renderRect;		//이미지를 위해서 만든 렉트
	float _count;			//시간을 재기 위해서 만든 카운트 변수.

	bool _attacked;			//공격의 성공여부를 측정하기 위해서 만든 불 변수.
	bool _attackedColor;	//피격시 적, 흰으로 반짝임을 넣어야 하기 때문에 만들은 불 변수.
	//에너미 방향상태 변수
	MoveType _move;		//방향에 관련된 이넘을 불러오기 위한 함수

	StateType _state;	//상태와 관련된 이넘을 불러오기 위한 함수

	//다른 사람의 클래스 불러오기
	class Player* _player;

public:

	//내 충돌에 대해서 누나가 처리를 할 수 있게 만들어놓은 내 hp를 깎는 함수.
	//충돌처리를 하고 싶을때 누나가 이 함수를 불러서 안에 데미지만 넣어주면 누나의 데미지가 나에게 들어와서 내 hp를 깎아줌.
	virtual void AttackedDemege(int damage);

	//렉트 생성을 위한 함수 메인렉트 이외에도 어려 렉트를 코드를 간략화 하고 싶어서 만든 함수.
	RECT UpdateRect(Vector2 position, Vector2 size, Pivot::Enum _pivot);

	//에너미 움직이는 타입. 앵글값에 따라서 이미지의 바라보는 방향이나 움직일 장소를 고를 수 있음.
	//각도로 잡아놔서 조금더 자연스럽게 움직일 수 있음.
	void EnemyMoveType();

	//이건 순우의 콜리젼 렉트를 사용하려면 내 렉트들을 전부 다 바꿔야 해서
	//가상함수를 이용해서 내쪽에서 인자값을 바꿔서 내가 렌더링한 렉트와 충돌처리를 시키려고 만든 함수.
	RECT GetCollisionRect()const override {return _renderRect;}

	//자신이외의 모든것들과 충돌처리를 하고 싶을때 사용.
	//약간의 식의 변형만 가진다면 충돌처리를 하면안되는것도 분류할 수 있음.
	//아래의 InterSectReaction과 연관되어있다.
	void ObjectCollision();

	//이건 형도 아는 충돌식인데 에너미 or 직접 움직이는 물체와 그 이외의 것들이랑 충돌할때 사용함.
	//지정해서 충돌할 상대를 정할때 사용한다고 생각하면 편할 듯
	bool IntersectReaction(RECT* moveRect, RECT* unMoveRect);

	//에너미는 항상 플레이어를 중심으로 돌아가기 때문에 플레이어와의 직선거리를 편하게 재기 위해서 만들어 놨음.
	float Distance(Vector2 pos);

	//디스턴스와 마찬가지로 편하게  플레이어를 편하게 쫒기 위해서 만들어 놓은 앵글함수.
	float Angle(Vector2 angle);

public:
	virtual void Init();
	virtual void Release() {}
	virtual void Update() {}
	virtual void Render() {}

	void SprayItemEnemy(string name);	//에너미 아이템 뿌리기 함수

public:
	Enemy();
	~Enemy();
};
