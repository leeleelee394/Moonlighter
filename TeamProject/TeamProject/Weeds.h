#pragma once
#include "Enemy.h"
class Weeds :	public Enemy
{
private:
	class Image* _weeds;
	class Image* _weeds_Red;
	class Image* _weeds_White;
	class Image* _shadow;
	int _imageCount;
	float _count;
	bool _isAttack;
	int	 _attackedCount;		//피격시 뒤로 밀리는 타이밍을 재기 위한 카운트
	float _attackTime;
	float _countMove;
	int _moveCount;
	bool _isStop;
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
	void Collision();
	void Move();
	void InvenStop(bool stop);
	void AttackedDemege(int damage)override;
public:
	Weeds(Vector2 pos);
	~Weeds();
};

