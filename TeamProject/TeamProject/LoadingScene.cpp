#include "stdafx.h"
#include "LoadingScene.h"

#include "Loading.h"
#include "Animation.h"
#include "Player.h"

LoadingScene::LoadingScene()
	:_loadingWString(L"로딩중"), _frameCount(0.f)
{
	_loadingImage = _ImageManager->AddFrameImage("loading", PathResources(L"Scene/loading.png"),34,1);
	_loadingAnimation = new Animation; 
	_loadingAnimation->SetStartEndFrame(0, 0, 33, 0, true);
	_loadingAnimation->SetFrameUpdateTime(0.1f);
	_loadingAnimation->SetIsLoop(true);
	_loadingAnimation->Play();
}


LoadingScene::~LoadingScene()
{
	SafeDelete(_loadingAnimation);
}

void LoadingScene::Init()
{
	_delayTime = 0.f;

	_loadingWString = L"로딩중";

	if (_loading)
		_loading->Start();
}

void LoadingScene::Release()
{
	SafeDelete(_loading);
}

void LoadingScene::Update()
{
	float deltaTime = _TimeManager->DeltaTime();
	_frameCount += deltaTime;
	_delayTime += deltaTime;
	if (_frameCount >= 1.0f)
	{
		while (_frameCount >= 1.0f)
		{
			_frameCount -= 1.0f;
		}
		if (_loadingWString.length() <= 6)
		{
			_loadingWString += L".";
		}
		else
		{
			_loadingWString = L"로딩중";
		}
	}

	if (_loading->GetIsLoadEnd() == true && _delayTime >= 1.0f)
	{
		_SceneManager->LoadScene(_nextSceneName, false);
	}

	_loadingAnimation->UpdateFrame();

}

void LoadingScene::Render()
{
	_DXRenderer->FillRectangle(Figure::RectMake(0, 0, WinSizeX, WinSizeY));

	_DXRenderer->RenderTextField(WinSizeX / 2 - _loadingWString.size() /2 * 10, WinSizeY / 2 + 200, 
		_loadingWString, 30, 300, 100, DefaultBrush::white, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);

	_loadingImage->SetSize(_loadingImage->GetFrameSize(0));
	_loadingImage->SetScale(1.3f);
	_loadingImage->FrameRender(929,590, _loadingAnimation->GetNowFrameX(), 0, Pivot::CENTER, false);

	//ImGui::Begin("Mouse");
	//ImGui::Text("X : %d", _Input->GetMousePositionPoint().x);
	//ImGui::Text("Y : %d", _Input->GetMousePositionPoint().y);
	//ImGui::End();

}
