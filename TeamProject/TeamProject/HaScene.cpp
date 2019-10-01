#include "stdafx.h"
#include "HaScene.h"


void HaScene::Init()
{
	_ObjectManager->Init();
}
void HaScene::Release()
{
	_ObjectManager->Release();
}
void HaScene::Update()
{
	_ObjectManager->Update();
}
void HaScene::Render()
{
	_ObjectManager->Render();
}

//持失切.
HaScene::HaScene()
{
}
//社瑚切.
HaScene::~HaScene()
{
}
