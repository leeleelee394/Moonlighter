#pragma once
#include "SceneBase.h"
class DungeonLobby : public SceneBase
{
private:
	class Image* _background;
public:
	DungeonLobby() {}
	~DungeonLobby() {}

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

