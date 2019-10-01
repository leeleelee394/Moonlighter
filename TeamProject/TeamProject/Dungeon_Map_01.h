#pragma once
#include "SceneBase.h"
class Dungeon_Map_01 :	public SceneBase
{
private:
	RECT collisionRc;
	class Image* map;
	class GameObject* _object;
private:
	void HaInit();
	void SoonInit();
	void ChanInit();
	void ReeInit();
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
public:
	Dungeon_Map_01();
	~Dungeon_Map_01();
};

