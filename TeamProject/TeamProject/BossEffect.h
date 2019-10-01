#pragma once
#include "GameObject.h"
class BossEffect : public GameObject
{
private:
	class Image* _bossScrollIMG;			//보스 스크롤 이미지
	class Animation* _bossScrollAnimation;	//보스 스크롤 애니메이션

	float _alpha;							//알파 값 변수

public:
	BossEffect();
	~BossEffect();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
};

