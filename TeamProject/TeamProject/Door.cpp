#include "stdafx.h"
#include "Door.h"
#include "Image.h"
#include "Player.h"
#include "Dungeon_Map_01.h"
#include "Dungeon_Map_02.h"
#include "LoadingScene.h"

string Door::_mapName = "";

Door::Door(Vector2 pos, Vector2 size)
{
	_name = "Door";
	_position = pos;
	_size = size;
	_pivot = Pivot::LEFT_TOP;
	this->UpdateMainRect();
	_timeCount = 0;
	_doorCount = 0;
	_door = _ImageManager->AddFrameImage("door", L"../Resources/Object/door.png", 5, 1);
	rcPosition = Vector2(_position.x + 50, _position.y + 40);
	rcSize = Vector2(_size.x - 100, _size.y - 30);
	rc = Figure::RectMake(rcPosition, rcSize);
	this->testCollision = Figure::RectMake(Vector2(0, 0), Vector2(WinSizeX, WinSizeY));
	_isDoorChange = false;
}


Door::~Door()
{
}

void Door::Init()
{
	
	this->_player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");
}

void Door::Release()
{
}

void Door::Update()
{
	RECT collisionRc;

	_timeCount += _TimeManager->DeltaTime();
	if (_timeCount > 0.2f)
	{
		_timeCount = 0;
		_doorCount++;
	}
	if (_doorCount >= 4)
	{
		_doorCount = 4;
	}

	if (IntersectRect(&collisionRc, &rc, &_player->GetCollisionRect()))
	{
		if (_doorCount == 4)
		{
			LoadingScene* loadingScene = dynamic_cast<LoadingScene*>(_SceneManager->FindScene("LoadingScene"));
			if (loadingScene != nullptr)
			{
				loadingScene->SetNextSceneName(Door::_mapName);
				loadingScene->SetLoadingFunc([]() {
					_SceneManager->FindScene(Door::_mapName)->Init();
				});
				_SceneManager->LoadScene("LoadingScene");
				_SoundManager->FadeoutBGM();
				return;
			}
			_doorCount = 0;
		}
	}
}

void Door::Render()
{
	_door->SetSize(_door->GetFrameSize(0));
	_door->SetScale(1.0f);
	_door->FrameRender(_position.x, _position.y, _doorCount, 0, Pivot::LEFT_TOP, true);
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(rc, DefaultBrush::red, true);
	}
}
