#include "stdafx.h"
#include "TitleFlowObject.h"

#include "LoadingScene.h"
TitleFlowObject::TitleFlowObject()
	:GameObject("TitleFlowObject",Vector2(),Vector2(),Pivot::CENTER)
{
	_state = State::Close;
	_selectIndex = 0;
	this->AddCallbackMessage("DoorOpened", [this](TagMessage msg) 
	{
		this->DoorOpened();
		_SoundManager->PlayBGM("introBGM");
	});

	_selectorList[0] = new TitleSelector(L"새로운 게임", Figure::RectMakeCenter(WinSizeX / 2, WinSizeY / 2 + 100,200,100), []() 
	{ 
		_SoundManager->Stop("introBGM");

		LoadingScene* loadingScene = dynamic_cast<LoadingScene*>(_SceneManager->FindScene("LoadingScene"));
		if (loadingScene != nullptr)
		{
			_ObjectManager->ChangeZOrdering(true);
			loadingScene->SetNextSceneName("MainScene");
			loadingScene->SetLoadingFunc([]() {
				_SceneManager->FindScene("MainScene")->Init();
			});
			_SceneManager->LoadScene("LoadingScene");
			_LightingSystem->ChangeState(LightSystem::State::Afternoon);
			return;
		}

	});
	_selectorList[1] = new TitleSelector(L"나가기", Figure::RectMakeCenter(WinSizeX / 2, WinSizeY / 2 + 200, 200, 100), []() { exit(0); });

	_selectorImage = _ImageManager->FindImage("Selector");
}


TitleFlowObject::~TitleFlowObject()
{
	for (UINT i = 0; i < ARRAYSIZE(_selectorList); ++i)
	{
		SafeDelete(_selectorList[i]);
	}
}

void TitleFlowObject::Update()
{
	GameObject::Update();

	switch (_state)
	{
	case TitleFlowObject::State::Close:
	{
		if (_Input->GetKeyDown(VK_SPACE))
		{
			vector<GameObject*> doorList = _ObjectManager->FindObjects(ObjectType::Object, "TitleDoor");
			for (UINT i = 0; i < doorList.size(); ++i)
				doorList[i]->SendCallbackMessage(TagMessage("Open"));
			_state = State::OpenSlide;
			_SoundManager->Play("DoorOpen");
		}
	}
		break;
	case TitleFlowObject::State::Open:
		if (_Input->GetKeyDown('S'))
		{
			_selectIndex = ++_selectIndex % 2;
		}
		if (_Input->GetKeyDown('W'))
		{
			--_selectIndex;
			if (_selectIndex < 0)
				_selectIndex = 1;
		}
		if (_Input->GetKeyDown(VK_SPACE))
		{
			_selectorList[_selectIndex]->func();
		}
		break;
	default:
		break;
	}
}

void TitleFlowObject::Render()
{
	if (_state == TitleFlowObject::State::Open)
	{
		for (UINT i = 0; i < ARRAYSIZE(_selectorList); ++i)
		{
			Vector2 size(_selectorList[i]->rc.right - _selectorList[i]->rc.left, _selectorList[i]->rc.bottom - _selectorList[i]->rc.top);
			_DXRenderer->RenderTextField(_selectorList[i]->rc.left, _selectorList[i]->rc.top,
				_selectorList[i]->name, 30, CastingInt(size.x), CastingInt(size.y), DefaultBrush::white, DWRITE_TEXT_ALIGNMENT_CENTER);
		}

		Vector2 leftPos(_selectorList[_selectIndex]->rc.left - _selectorImage->GetWidth() / 2.f,
			_selectorList[_selectIndex]->rc.top + (_selectorList[_selectIndex]->rc.bottom - _selectorList[_selectIndex]->rc.top) / 2.0f);
		Vector2 rightPos(_selectorList[_selectIndex]->rc.right + _selectorImage->GetWidth() / 2.f,
			_selectorList[_selectIndex]->rc.top + (_selectorList[_selectIndex]->rc.bottom - _selectorList[_selectIndex]->rc.top) / 2.0f);
		_selectorImage->Render(CastingInt(leftPos.x), CastingInt(leftPos.y), Pivot::CENTER);
		_selectorImage->Render(CastingInt(rightPos.x), CastingInt(rightPos.y), Pivot::CENTER);

	}
}

void TitleFlowObject::DoorOpened()
{
	this->_state = State::Open;
}
