#include "stdafx.h"
#include "TownScene.h"

#include "GameObject.h"
TownScene::TownScene()
{
}


TownScene::~TownScene()
{
}

void TownScene::Init()
{
	_townBackgroundImage = _ImageManager->AddImage("TownBackground", PathResources(L"Town/map.bmp"));

	this->SoonInit();
	this->ReeInit();
	this->HaInit();
	this->ChanInit();

	_ObjectManager->Init();
	_ObjectManager->ChangeZOrdering(true);

	GameObject* player = _ObjectManager->FindObject(ObjectType::Object, "Will");
	if (player)
	{
		player->SetPosition(Vector2(_Database->GetVector2Data("PlayerPosition")));
	}

	_Camera->SetMapSize(Vector2((float)_townBackgroundImage->GetWidth(),(float) _townBackgroundImage->GetHeight()));
	_Camera->SetCameraMoveDistance();
	_Camera->InitCameraToPlayer();
	//_Camera->SetState(CameraManager::MoveState::FreeCamera);

	_SoundManager->PlayBGM("townBGM");

	if(_LightingSystem->GetState() == LightSystem::State::Evening)
		_LightingSystem->ChangeState(LightSystem::State::Night);
}

void TownScene::Release()
{
	_ObjectManager->Release();
}

void TownScene::Update()
{
	_ObjectManager->Update();
}

void TownScene::Render()
{
	_townBackgroundImage->Render(0, 0, Pivot::LEFT_TOP, true);

	_ObjectManager->Render();

}

