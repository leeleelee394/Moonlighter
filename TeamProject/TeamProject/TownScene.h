#pragma once
#include "SceneBase.h"
class TownScene : public SceneBase
{
private:
	class Image* _townBackgroundImage;
public:
	TownScene();
	~TownScene();

	void Init()override;
	void Release()override; 
	void Update()override; 
	void Render()override; 

private:
	void ChanInit(); 
	void HaInit(); 
	void SoonInit(); 
	void ReeInit(); 
};

