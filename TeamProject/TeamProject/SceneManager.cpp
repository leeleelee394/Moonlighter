#include "stdafx.h"
#include "SceneManager.h"
#include "SceneBase.h"
#include "LoadingScene.h"

SingletonCpp(SceneManager)
/***************************************************************
## SceneManager ##
***************************************************************/
SceneManager::SceneManager()
	:nowScene(nullptr), loadFunc(nullptr), state(State::None), fadeAlpha(1.f)
{

}
/***************************************************************
## ~SceneManager ##
***************************************************************/
SceneManager::~SceneManager()
{
	SceneIter iter = sceneList.begin();
	for (; iter != sceneList.end(); ++iter)
	{
		iter->second->Release();
		SafeDelete(iter->second);
	}
	sceneList.clear();
}

/***************************************************************
## Update ##
***************************************************************/
void SceneManager::Update()
{
	if (nowScene)
		nowScene->Update();

	switch (state)
	{
	case SceneManager::State::Load:
		break;
	case SceneManager::State::None:
		break;
	case SceneManager::State::FadeOut:
		fadeAlpha += 1.0f * _TimeManager->DeltaTime();
		if (fadeAlpha >= 1.0f)
		{
			fadeAlpha = 1.0f; 
			state = State::None;
		}
		break;
	case SceneManager::State::FadeIn:
		fadeAlpha -= 1.0f * _TimeManager->DeltaTime();
		if (fadeAlpha <= 0.f)
		{
			fadeAlpha = 0.f;
			state = State::None;
		}
		break;
	default:
		break;
	}
}
/***************************************************************
## Render ##
***************************************************************/
void SceneManager::Render()
{
	if (nowScene)
		nowScene->Render();
	if(FLOAT_EQUAL(fadeAlpha,0.f) == false)
		_DXRenderer->FillRectangle({ 0,0,WinSizeX,WinSizeY }, D2D1::ColorF::Black, fadeAlpha, false);
}

void SceneManager::SceneQueue()
{
	if (state == State::None)
	{
		if (loadFunc != nullptr)
		{
			loadFunc(loadSceneName, bInit);
			loadFunc = nullptr;
		}
	}
}

/***************************************************************
## AddScene ##
@@ string name : ¾À ÀÌ¸§ 
@@ ¹Ù²Ü ¾À
***************************************************************/
void SceneManager::AddScene(string name, SceneBase * scene)
{
	SceneIter iter = sceneList.find(name);
	if (iter != sceneList.end())
		return;
	this->sceneList.insert(make_pair(name, scene));
}
/***************************************************************
## FindScene ##
@@ string name : Ã£À» ¾ÀÀÌ¸§ 
***************************************************************/
SceneBase * SceneManager::FindScene(string name)
{
	SceneIter iter = sceneList.find(name);
	if (iter != sceneList.end())
		return iter->second;

	return nullptr;
}
/***************************************************************
## LoadScene ##
@@ string name : ¹Ù²Ü ¾ÀÀÌ¸§ 
@@ bool init : ¹Ù²Ü ¾À InitÇÒ ¿©ºÎ
***************************************************************/
void SceneManager::ChangeScene(string name, bool init)
{
 	SceneBase* findScene = this->FindScene(name);
	if (findScene != nullptr)
	{
		if (nowScene)
			nowScene->Release();

		nowScene = findScene;

		if (init)
			nowScene->Init();

		this->state = State::FadeIn;
	}
}

void SceneManager::LoadScene(string name,bool init)
{
	this->loadFunc = bind(&SceneManager::ChangeScene, this, name, init);

	this->loadSceneName = name;
	this->bInit = init;
	this->state = State::FadeOut;
}

void SceneManager::LoadSceneByLoading(string name)
{
	LoadingScene* loadingScene = dynamic_cast<LoadingScene*>(_SceneManager->FindScene("LoadingScene"));
	if (loadingScene != nullptr)
	{
		this->nextSceneName = name;
		_ObjectManager->ChangeZOrdering(true);
		loadingScene->SetNextSceneName(nextSceneName);
		loadingScene->SetLoadingFunc([this]() 
		{
			_SceneManager->FindScene(nextSceneName)->Init();
		});
		_SceneManager->LoadScene("LoadingScene");
		return;
	}
}
