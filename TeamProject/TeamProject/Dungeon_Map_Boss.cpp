#include "stdafx.h"
#include "Dungeon_Map_Boss.h"
#include "Door.h"

#include "EventManager.h"
#include "InterfaceClass.h"
Dungeon_Map_Boss::Dungeon_Map_Boss()
{
}


Dungeon_Map_Boss::~Dungeon_Map_Boss()
{
}

void Dungeon_Map_Boss::Init()
{
	this->HaInit();
	this->ReeInit();
	this->ChanInit();
	this->SoonInit();

	_ObjectManager->Init();

	Door::_mapName = "Dungeon_Map_Boss";

	GameObject* _player = _ObjectManager->FindObject(ObjectType::Object, "Will");
	if (_player)
	{
		_player->SetPosition(Vector2(2100 / 2, 1421 - 200));
	}
	_map = _ImageManager->AddImage("BoosRoom", L"../Resources/Scene/BossRoom.png", true);
	_door = _ImageManager->AddFrameImage("doorDown", L"../Resources/Object/doordown.png", 5, 1, true);
	_Camera->SetMapSize(Vector2((float)_map->GetWidth(), (float)_map->GetHeight()));
	_Camera->InitCameraToPlayer();
	_LightingSystem->ChangeState(LightSystem::State::Afternoon);

	_SoundManager->PlayBGM("GolemKingRoom");

	//_ObjectManager->AddObject(ObjectType::Object, new GameObject("Empty", Vector2(1070, 340), Vector2(1, 1), Pivot::CENTER));
	//_EventManager->PushEvent(new IEventCameraMove(_ObjectManager->FindObject(ObjectType::Object, "Empty")));
	//_EventManager->PushEvent(new IEventDelay(3.f));
	//_EventManager->PushEvent(new IEventCameraMove(_player));
}

void Dungeon_Map_Boss::Release()
{
	_ObjectManager->Release();
}

void Dungeon_Map_Boss::Update()
{
	_ObjectManager->Update();

	_EventManager->Update();
}

void Dungeon_Map_Boss::Render()
{
	_door->SetSize(_door->GetFrameSize(0));
	_door->SetScale(1.0f);
	_map->Render(0, 0, Pivot::LEFT_TOP, true);
	_door->FrameRender(1050, 1350, 0, 0, Pivot::CENTER, true);
	_ObjectManager->Render();
}
