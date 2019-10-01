#include "stdafx.h"
#include "SceneChangeObject.h"


SceneChangeObject::SceneChangeObject(RECT rc, function<void()> func)
{
	_collider = rc;
	this->_mainRect = { rc .left + 1,rc .top + 1, rc .right - 1, rc .bottom - 1};
	this->_func = func;
}

SceneChangeObject::~SceneChangeObject()
{
}

void SceneChangeObject::Init()
{
	this->_player = _ObjectManager->FindObject(ObjectType::Object, "Will");
}

void SceneChangeObject::Update()
{
	if (_player)
	{
		RECT temp; 
		if (IntersectRect(&temp, &_collider, &_player->GetCollisionRect()))
		{
			if(this->_func)
				this->_func();
			_SoundManager->FadeoutBGM();
		}
	}
}
