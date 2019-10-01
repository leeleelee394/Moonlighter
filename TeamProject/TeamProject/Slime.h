#pragma once
#include "Enemy.h"
class Slime :
	public Enemy
{
	//쪼꼬미로 돌아다니다가 플레이어를 발견하면 폴짝뛰어서 장거리 이동을 하고 공격하는거랑 
	//폴짝뛰어서 플레이어에게 붙어서 플레이어를 일정시간동안 가두고 그사이시간동안 도트데미지를 주는 것.
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
public:
	Slime();
	~Slime();
};

