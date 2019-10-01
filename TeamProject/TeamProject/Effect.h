#pragma once
#include "GameObject.h"

//소드 공격 이팩트 키값
#define EFFECT_SWORDATK "swordAtkEffect"
//활 공격 이팩트 키값
#define EFFECT_BOWATK "bowAtkEffect"
//폭발 이팩트 키값
#define EFFECT_BOOM "boomEffect"
class Effect : public GameObject
{
public:
	//이펙트 재생 요청(비활성 이펙트를 찾은 뒤 이펙트를 재생한다)
	static void PlayEffect(const string name,const Vector2 position);
private:
	class Image* _effectIMG;					//이팩트 이미지
	class Animation* _effectAnimation;			//애니메이션
public:
	Effect();
	virtual ~Effect();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
	//이펙트 재생
	void StartEffect(const string name,const Vector2 position);
};

