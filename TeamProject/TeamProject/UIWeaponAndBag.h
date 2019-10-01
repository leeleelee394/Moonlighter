#pragma once
#include "GameObject.h"
class UIWeaponAndBag : public GameObject
{
private:
	class Image* _uIWeaponSwapIMG;			//무기 스왑 이미지
	class Image* _uIBagIMG;					//가방 이미지 

	class Animation* _uIswapAnimation;		//무기 스왑 애니메이션 선언

	bool _isSwap;							//스왑 상태 bool

	int _swapCount;							//스왑 카운트


public:
	UIWeaponAndBag();
	~UIWeaponAndBag();

	void Init();
	void Release();
	void Update();
	void Render();
};

