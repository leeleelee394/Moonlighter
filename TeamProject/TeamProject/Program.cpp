#include "stdafx.h"
#include "Program.h"

#include "MainScene.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "TownScene.h"
#include "Dungeon_Map_01.h"
#include "Dungeon_Map_02.h"
#include "Dungeon_Map_Boss.h"
#include "DebugSystem.h"
#include "DungeonLobby.h"

Program::Program()
{
	
}

Program::~Program()
{
}

void Program::Init()
{
	_SceneManager->AddScene("MainScene", new MainScene);
	_SceneManager->AddScene("TitleScene", new TitleScene);
	_SceneManager->AddScene("TownScene", new TownScene);
	_SceneManager->AddScene("Dungeon_Map_01", new Dungeon_Map_01);
	_SceneManager->AddScene("Dungeon_map_02", new Dungeon_Map_02);
	_SceneManager->AddScene("Dungeon_Map_Boss", new Dungeon_Map_Boss);
	_SceneManager->AddScene("DungeonLobby", new DungeonLobby);

	LoadingScene* loadScene = new LoadingScene;
	loadScene->SetNextSceneName("TitleScene");
	loadScene->SetLoadingFunc([this]() 
	{
		this->LoadResourceSoon();
		this->LoadResourceChan();
		this->LoadResourceRee();
		this->LoadResourceHa();
		_SceneManager->FindScene("TitleScene")->Init(); 
	});
	
	_SceneManager->AddScene("LoadingScene", loadScene);

	_SceneManager->LoadScene("LoadingScene");

	_SceneManager->SceneQueue();	

	_SoundManager->SetMusicVolume(0.3f);
	//1950,686
	//_Database->AddVector2Data("PlayerPosition", Vector2(1950, 686));
	_Database->AddVector2Data("PlayerPosition", Vector2(1050, 686));
	_Database->AddIntData("GateState", 0);
	_Database->AddIntData("PlayerActive", 1);
	_Database->AddIntData("PlayerHP", 100);
	_Camera->SetState(CameraManager::MoveState::End);
}

void Program::Release()
{
	
}

void Program::Update()
{
	if (_Input->GetKeyDown(VK_F1))
		_isDebug = !_isDebug;
	if (_Input->GetKeyDown(VK_F2))
		_LightingSystem->ChangeState(LightSystem::State::Afternoon);
	else if (_Input->GetKeyDown(VK_F3))
		_LightingSystem->ChangeState(LightSystem::State::Evening);
	else if (_Input->GetKeyDown(VK_F4))
		_LightingSystem->ChangeState(LightSystem::State::Night);


	_SceneManager->Update();
}

void Program::Render()
{
	_DXRenderer->BindSwapchainRenderTargetAndClear();
	_DXRenderer->Direct2DBeginDraw();
	{
		_SceneManager->Render();
		if (_isDebug)
			DebugSystem::DebugRender();
	}
	_DXRenderer->Direct2DEndDraw();
	ImGui::Render();
	_DXRenderer->PresentSwapChain();

	_SceneManager->SceneQueue();
}


