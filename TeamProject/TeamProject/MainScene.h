#pragma once
#include "SceneBase.h"
class MainScene : public SceneBase
{
public:
	MainScene() {}
	~MainScene() {}
	
	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 

private:
	void ChanInit();
	void HaInit();
	void ReeInit();
	void SoonInit();
};

