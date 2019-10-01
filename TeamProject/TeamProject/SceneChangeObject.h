#pragma once
#include "GameObject.h"
class SceneChangeObject : public GameObject
{
private:
	function<void()> _func;
	class GameObject* _player;
	RECT _collider;
public:
	SceneChangeObject(RECT rc,function<void()> func);
	~SceneChangeObject();

	void Init()override; 
	void Update()override; 

};

