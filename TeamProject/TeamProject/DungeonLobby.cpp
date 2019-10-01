#include "stdafx.h"
#include "DungeonLobby.h"

#include "GameObject.h"
void DungeonLobby::Init()
{
	_background = _ImageManager->FindImage("Dungeon_Lobby");
	
	this->SoonInit();
	this->ReeInit();
	this->HaInit();
	this->ChanInit();

	_ObjectManager->Init();

	GameObject* player = _ObjectManager->FindObject(ObjectType::Object, "Will");
	if (player)
	{
		player->SetPosition(Vector2(_Database->GetVector2Data("PlayerPosition")));
	}

	_Camera->SetMapSize(Vector2(CastingInt(_background->GetWidth()), CastingInt(_background->GetHeight())));
	_Camera->SetCameraMoveDistance();
	_Camera->InitCameraToPlayer();

	_SoundManager->PlayBGM("poolRoomBGM");
}

void DungeonLobby::Release()
{
	_ObjectManager->Release();
}

void DungeonLobby::Update()
{
	_ObjectManager->Update();
}

void DungeonLobby::Render()
{
	_background->Render(0, 0, Pivot::LEFT_TOP, true);
	_ObjectManager->Render();
}

void DungeonLobby::HaInit()
{
}

void DungeonLobby::ReeInit()
{
}
