#pragma once
#include "SceneBase.h"
class Dungeon_Map_Boss : public SceneBase
{
private:
	class Image* _map;
	class Image* _door;
	class GameObject* _object;
public:
	void HaInit();
	void ReeInit();
	void ChanInit();
	void SoonInit();
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
public:
	Dungeon_Map_Boss();
	~Dungeon_Map_Boss();
};

