#include "stdafx.h"
#include "Dungeon_Map_01.h"
#include "Image.h"
#include "GameObject.h"
#include "LoadingScene.h"
#include "Door.h"
void Dungeon_Map_01::Init()
{
	GameObject* object = new GameObject;
	object->SetPosition(Vector2(0, 0));
	object->SetSize(Vector2(WinSizeX, 90));
	object->SetPivot(Pivot::LEFT_TOP);
	_ObjectManager->AddObject(ObjectType::Object, object);

	GameObject* object1 = new GameObject;
	object1->SetPosition(Vector2(0, WinSizeY - 90));
	object1->SetSize(Vector2(WinSizeX, 90));
	object1->SetPivot(Pivot::LEFT_TOP);
	_ObjectManager->AddObject(ObjectType::Object, object1);

	GameObject* object2 = new GameObject;
	object2->SetPosition(Vector2(0, 0));
	object2->SetSize(Vector2(90, WinSizeY));
	object2->SetPivot(Pivot::LEFT_TOP);
	_ObjectManager->AddObject(ObjectType::Object, object2);

	GameObject* object3 = new GameObject;
	object3->SetPosition(Vector2(WinSizeX - 90, 0));
	object3->SetSize(Vector2(90, WinSizeY));
	object3->SetPivot(Pivot::LEFT_TOP);
	_ObjectManager->AddObject(ObjectType::Object, object3);

	Door* _door = new Door(Vector2((WinSizeX / 2 - 90), 0), Vector2(155, 100));
	Door::_mapName = "Dungeon_map_02";
	_ObjectManager->AddObject(ObjectType::Object, _door);

	map = _ImageManager->AddImage("mpa01", L"../Resources/Scene/Dungeon_Map_01.png");

	this->HaInit();
	this->SoonInit();
	this->ChanInit();
	this->ReeInit();
	
	_ObjectManager->Init();
	_Camera->SetMapSize(Vector2((int)map->GetWidth(),(int) map->GetHeight()));
	_Camera->InitCameraToPlayer();
	_Camera->SetState(CameraManager::MoveState::End);

	_SoundManager->PlayBGM("dungeonBGM");
	_LightingSystem->ChangeState(LightSystem::State::Afternoon);
}

void Dungeon_Map_01::Release()
{
	_ObjectManager->Release();
}

void Dungeon_Map_01::Update()
{
	_ObjectManager->Update();
}

void Dungeon_Map_01::Render()
{
	map->Render(3, 3, Pivot::LEFT_TOP, true);
	_ObjectManager->Render();
}

Dungeon_Map_01::Dungeon_Map_01()
{
	

}


Dungeon_Map_01::~Dungeon_Map_01()
{
}
