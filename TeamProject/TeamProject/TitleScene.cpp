#include "stdafx.h"
#include "TitleScene.h"

#include "LoadingScene.h"
#include "TitleDoor.h"
#include "TitleBackground.h"
#include "TitleLogo.h"
#include "TitleFlowObject.h"
#include "PointLight.h"

TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	_ObjectManager->AddObject(ObjectType::BackGround, new TitleBackground);
	_ObjectManager->AddObject(ObjectType::Object, new TitleDoor("LeftDoor", Vector2(-1, 0), Vector2(-4, 0), Vector2(WinSizeX / 2, WinSizeY)));
	_ObjectManager->AddObject(ObjectType::Object, new TitleDoor("RightDoor", Vector2(1, 0), Vector2(WinSizeX / 2 + 4, 0), Vector2(WinSizeX / 2, WinSizeY)));
	_ObjectManager->AddObject(ObjectType::Object, new TitleLogo);
	_ObjectManager->AddObject(ObjectType::Object, new TitleFlowObject);
	_ObjectManager->AddObject(ObjectType::UI, new PointLight(Vector2(WinSizeX / 2, 0), 400.f, 1.7f, GameColor(1.f, 1.f, 1.f, 1.f)));

	_ObjectManager->Init();
}

void TitleScene::Release()
{
	_ObjectManager->Release();
}

void TitleScene::Update()
{
	_ObjectManager->Update();
}

void TitleScene::Render()
{
	_ObjectManager->Render();

}
