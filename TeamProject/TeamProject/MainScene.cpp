#include "stdafx.h"
#include "MainScene.h"
#include "GameObject.h"
#include "Inventory.h"
#include "LoadingScene.h"

void MainScene::Init()
{
	this->SoonInit();
	this->ReeInit();
	this->ChanInit();
	//this->HaInit();

	_ObjectManager->Init();

	_Camera->SetTarget(_ObjectManager->FindObject(ObjectType::Object, "Will"));
	_Camera->SetMapSize(Vector2(3000, 3000));

}

void MainScene::Release()
{
	_ObjectManager->Release();
}

void MainScene::Update()
{
	_ObjectManager->Update();

}

void MainScene::Render()
{
	_ObjectManager->Render();
}
