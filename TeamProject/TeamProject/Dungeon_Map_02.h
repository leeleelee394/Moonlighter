#pragma once
#include "SceneBase.h"
class Dungeon_Map_02 :	public SceneBase
{
private:
	RECT _doorRect;
	float _timeCount;
	int _closeDoorCount;
	bool _isTransScene;
	
	class Image* _map;
	
	class GameObject* _object;
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void ChanInit();
	void HaInit();
	void ReeInit();
	void SoonInit();

public:
	Dungeon_Map_02();
	~Dungeon_Map_02();
};