#include "stdafx.h"
#include "DungeonGate.h"

#include "Animation.h"

DungeonGate::DungeonGate()
	:_mainImage(nullptr),_mainAnimaition(nullptr)
{
	Image* sizeImage = _ImageManager->FindImage("dungeonLobby_doorOpen");;

	_mainRect = { 672,898, (LONG)(672 + sizeImage->GetFrameSize(0).x) + 2,(LONG)(898 + sizeImage->GetFrameSize(0).y) };
	_collider = { _mainRect.left,_collider.top,_collider.right,_collider.bottom - 10 };
	_position.x = _mainRect.left + (_mainRect.right - _mainRect.left)/2;
	_position.y = _mainRect.top + (_mainRect.bottom - _mainRect.top)/2;
	_size.x = _mainRect.right - _mainRect.left;
	_size.y = _mainRect.bottom - _mainRect.top;

	TagRenderInfo idleInfo;
	idleInfo.image = _ImageManager->FindImage("dungeonLobby_doorOpen");
	idleInfo.animation = new Animation;
	idleInfo.animation->SetIsLoop(false);
	_renderInfoList.insert(make_pair(State::Idle, idleInfo));

	TagRenderInfo openInfo;
	openInfo.image = _ImageManager->FindImage("dungeonLobby_doorOpen");
	openInfo.animation = new Animation;
	openInfo.animation->SetStartEndFrame(0, 0, 8, 0, false);
	openInfo.animation->SetFrameUpdateTime(0.1f);
	openInfo.animation->SetIsLoop(false);
	openInfo.animation->SetCallbackFunc([this]() 
	{
		this->ChangeState(State::OpenIdle);
	});
	_renderInfoList.insert(make_pair(State::Opening, openInfo));

	TagRenderInfo closingInfo;
	closingInfo.image = _ImageManager->FindImage("dungeonLobby_doorOpen");

	vector<pair<int, int>> frameList{ {9,0},{8,0} ,{7,0},{6,0},{5,0} ,{4,0}, {3,0},{2,0},{1,0},{0,0} };
	closingInfo.animation = new Animation;
	closingInfo.animation->SetVectorFrame(frameList);
	closingInfo.animation->SetFrameUpdateTime(0.1f);
	closingInfo.animation->SetIsLoop(false);
	closingInfo.animation->SetCallbackFunc([this]() 
	{
		this->ChangeState(State::Idle);
	});
	_renderInfoList.insert(make_pair(State::Closing, closingInfo));

	TagRenderInfo openIdleInfo;
	openIdleInfo.image = _ImageManager->FindImage("DungeonGateOpenIdle");
	openIdleInfo.animation = new Animation;
	openIdleInfo.animation->SetStartEndFrame(0, 0, 5, 0, false);
	openIdleInfo.animation->SetIsLoop(true);
	openIdleInfo.animation->SetFrameUpdateTime(0.1f);
	_renderInfoList.insert(make_pair(State::OpenIdle, openIdleInfo));

	TagRenderInfo enterInfo;
	enterInfo.image = _ImageManager->FindImage("dungeonLobby_goInDungeon");
	enterInfo.animation = new Animation;
	enterInfo.animation->SetStartEndFrame(0, 0, enterInfo.image->GetMaxFrameX() - 1, 0,false);
	enterInfo.animation->SetIsLoop(false);
	enterInfo.animation->SetFrameUpdateTime(0.1f);
	enterInfo.animation->SetCallbackFunc([this]() 
	{
		_SceneManager->LoadSceneByLoading("Dungeon_Map_01");
		this->ChangeState(State::End);
	});
	_renderInfoList.insert(make_pair(State::Enter, enterInfo));

	TagRenderInfo exitInfo;
	exitInfo.image = _ImageManager->FindImage("dungeonLobby_goOutDungeon");
	exitInfo.animation = new Animation;
	exitInfo.animation->SetStartEndFrame(0, 0, exitInfo.image->GetMaxFrameX() - 1, 0, false);
	exitInfo.animation->SetIsLoop(false);
	exitInfo.animation->SetFrameUpdateTime(0.1f);
	exitInfo.animation->SetCallbackFunc([this]() 
	{
		this->ChangeState(State::Idle);
		GameObject* object = _ObjectManager->FindObject(ObjectType::Object, "Will");
		object->SetActive(true);
	});
	_renderInfoList.insert(make_pair(State::Exit, exitInfo));

	this->AddCallbackMessage("PlaySound", [this](TagMessage message) 
	{
		_SoundManager->Play("dungeon_entrance_slime_door_spit");
	});

	this->ChangeState((State)_Database->GetIntData("GateState"));
}


DungeonGate::~DungeonGate()
{
	_Database->AddIntData("GateState", (int)_state);
}

void DungeonGate::Init()
{
	_player = _ObjectManager->FindObject(ObjectType::Object, "Will");
}

void DungeonGate::Release()
{
	RenderInfoIter iter = _renderInfoList.begin();
	for (; iter != _renderInfoList.end(); ++iter)
	{
		SafeDelete(iter->second.animation);
	}
	_renderInfoList.clear();
}

void DungeonGate::Update()
{
	if (_player)
	{
		GameObject::Update();
		switch (_state)
		{
		case DungeonGate::State::Idle:

			if (Math::GetDistance(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y) < 150.f)
			{
				if (_position.y < _player->GetPosition().y)
				{
					this->ChangeState(State::Opening);
				}
			}

			break;

		case DungeonGate::State::OpenIdle:
			if (Math::GetDistance(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y) > 150.f)
			{
				this->ChangeState(State::Closing);
			}
			else
			{
				if (Math::Abs(_position.x -  _player->GetPosition().x) < 20.f)
				{
					if (_Input->GetKeyDown('J'))
					{
						_player->SetActive(false);
						this->ChangeState(State::Enter);
					}
				}
			}

			break;
		case DungeonGate::State::Exit:
			break;
		default:
			break;
		}
		_mainAnimaition->UpdateFrame();
	}
}

void DungeonGate::Render()
{
	if (_state != State::Idle)
	{
		_mainImage->SetSize(_size);
		this->_mainImage->FrameRender(CastingInt(_mainRect.left), CastingInt(_mainRect.top), _mainAnimaition->GetNowFrameX(),
			_mainAnimaition->GetNowFrameY(), Pivot::LEFT_TOP, true);
	}
	else
	{
		_mainImage->SetSize(_size);
		this->_mainImage->FrameRender(CastingInt(_mainRect.left), CastingInt(_mainRect.top), 0,0, Pivot::LEFT_TOP, true);
	}

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
		_DXRenderer->DrawRectangle(_collider, DefaultBrush::red, true);
	}
}

void DungeonGate::ChangeState(State state)
{
	_state = state;
	this->ChangeRenderInfo(state);
	switch (_state)
	{
	case DungeonGate::State::Idle:
		_size.y = _mainImage->GetFrameSize(0).y;
		
		break;
	case DungeonGate::State::OpenIdle:
		_size.y = _mainImage->GetFrameSize(0).y;
		break;
	case DungeonGate::State::Closing:
		_size.y = _mainImage->GetFrameSize(0).y;
		break;
	case DungeonGate::State::Enter:
		_size.y = _mainImage->GetFrameSize(0).y;
		_SoundManager->Play("dungeon_entrance_slime_door_absorb");
		_SoundManager->FadeoutBGM();
		break;
	case DungeonGate::State::Exit:
		_size.y = _mainImage->GetFrameSize(0).y;
		_LightingSystem->ChangeState(LightSystem::State::Evening);
		this->SendCallbackMessage(TagMessage("PlaySound", 0.8f));
		
		break;
	default:
		break;
	}
}

void DungeonGate::ChangeRenderInfo(State state)
{
	RenderInfoIter iter = _renderInfoList.find(state);
	if (iter != _renderInfoList.end())
	{
		if (iter->second.image && iter->second.animation)
		{
			this->_mainImage = iter->second.image;
			this->_mainAnimaition = iter->second.animation;
			this->_mainAnimaition->Stop();
			this->_mainAnimaition->Play();
		}
	}
}
