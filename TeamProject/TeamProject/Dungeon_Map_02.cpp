#include "stdafx.h"
#include "Dungeon_Map_02.h"
#include "GameObject.h"
#include "Image.h"
#include "LoadingScene.h"
#include "Player.h"
#include "Door.h"

Dungeon_Map_02::Dungeon_Map_02()
{
	//단단단다라단단 단단단다라단단 뿌뿌~~ 딴따라헤이~ 따라단따헤헤이~
}


Dungeon_Map_02::~Dungeon_Map_02()
{
}

void Dungeon_Map_02::Init()
{
	_closeDoorCount = 0;
	_timeCount = 0.f;
	_isTransScene = false;
	
	_map = _ImageManager->AddImage("map02", L"../Resources/Scene/Dungeon_Map_02.png");

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
	Door::_mapName = "Dungeon_Map_Boss";
	_ObjectManager->AddObject(ObjectType::Object, _door);

	this->ChanInit();
	this->ReeInit();
	this->HaInit();
	this->SoonInit();

	_ObjectManager->Init();

	_Camera->SetMapSize(Vector2((int)_map->GetWidth(), (int)_map->GetHeight()));
	_Camera->InitCameraToPlayer();
	_Camera->SetState(CameraManager::MoveState::End);

	_SoundManager->SetMusicVolume(0.1f);
	_SoundManager->PlayBGM("dungeonBGM");
	_LightingSystem->ChangeState(LightSystem::State::Afternoon);
}

void Dungeon_Map_02::Release()
{
	_SoundManager->SetMusicVolume(0.3f);
	_ObjectManager->Release();
}
void Dungeon_Map_02::Update()
{

	_ObjectManager->Update();
}
void Dungeon_Map_02::Render()
{
	_map->Render(3,3, Pivot::LEFT_TOP, true);
	_ObjectManager->Render();
}

