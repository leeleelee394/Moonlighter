#pragma once
#include "Enemy.h"
class Plankton : public Enemy
{
private:
	enum class PlanktonState
	{
		Follow = 0,
		Attack,
		Attacked,
		Dead,
		End
	};

	class Image* _plankton;					//플랑크톤 몬스터 이미지
	class Image* _planktonRed;				//플랑크톤 빨강 (피격)
	class Image* _planktonWhite;			//플랑크톤 하양 (피격)

	class Animation* _planktonAnimation;	//플랑크톤 애니메이션

	PlanktonState _planktonState;			//이넘 스테이트 : 플랑크톤 상태

	bool _isAttack;							//공격 bool 값
	bool _isAtkedRender;					//피격 Render bool 값
	bool _isInvenStop;						//인벤토리 정지용 bool 값

	bool _isAtked;							//피격 bool 값

	RECT _atkRect;							//공격 렉트

	float _atkCount;						//공격 카운트

	float deltaTimeCount;					//델타 타임 카운트
	float atkedCount;						//피격 카운트

public:
	Plankton(Vector2 position);
	~Plankton();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void PlanktonStates();	//플랑크톤 상태 함수

	void AtkedState(Vector2 position, float angle, float speed, float countValue);

	void InvenStop(bool isInvenStop);	//인벤 정지용 함수
};




