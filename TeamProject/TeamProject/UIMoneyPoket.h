#pragma once
#include "GameObject.h"
class UIMoneyPoket : public GameObject
{
private:
	//이미지 클래스 선언 - 돈 주머니
	class Image* _moneyPoketIMG;
	


public:
	UIMoneyPoket();
	~UIMoneyPoket();

	void Init();
	void Release();
	void Update();
	void Render();
};

