#include "stdafx.h"
#include "GameObject.h"

#include <functional>
#include <unordered_map>

/*************************************************************************
## GameObject ##
**************************************************************************/
GameObject::GameObject()
	:_name(""), _pivot(Pivot::CENTER),_position(0,0),_size(0,0),_isActive(true),_isLive(true)
{
	this->UpdateMainRect();
}
/*************************************************************************
## GameObject ##
@@ string name : 객체 이름 
@@ Vector2 pos : 객체 좌표
@@ Vector2 size : 객체 사이즈 
@@ Pivot::Enum pivot : 객체 피봇 
**************************************************************************/
GameObject::GameObject(string name, Vector2 pos, Vector2 size, Pivot::Enum pivot)
	:_name(name),_position(pos),_size(size),_pivot(pivot),_isActive(true),_isLive(true)
{
	this->UpdateMainRect();
}

GameObject::GameObject(const RECT rect)
	:_name(""),_pivot(Pivot::LEFT_TOP),_isActive(true),_isLive(true)
{
	this->_mainRect = rect;
	this->_position = Vector2(_mainRect.left, _mainRect.top);
	this->_size = Vector2(_mainRect.right - _mainRect.left, _mainRect.bottom - _mainRect.top);
}

/*************************************************************************
## ~GameObject ##
**************************************************************************/
GameObject::~GameObject()
{
	this->_callbackList.clear();
	this->_reserveMessageList.clear();
}

/*************************************************************************
## Release ##
**************************************************************************/
void GameObject::Release()
{
	_reserveMessageList.clear();
}
/*************************************************************************
## Update ##
예약 메세지가 있다면 예약시간을 재다가 시간이 다되었다면 해당 메세지의 함수를 호출
**************************************************************************/
void GameObject::Update()
{
	float deltaTime = _TimeManager->DeltaTime();
	for (UINT i = 0; i < _reserveMessageList.size(); ++i)
	{
		//예약된 메세지의 예약시간을 deltaTime만큼 빼준다
		_reserveMessageList[i].delayTime -= deltaTime;
		//만약 예약된 시간이 다되었다면 
		if (_reserveMessageList[i].delayTime <= 0.f)
		{
			//해당 메세지의 함수를 실행시켜준다. 
			this->SendCallbackMessage(_reserveMessageList[i]);
			_reserveMessageList.erase(_reserveMessageList.begin() + i--);
		}	
	}
}
void GameObject::Render()
{
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);
	}
}
/*************************************************************************
## SendCallbackMessage ##
@@ TagMessage message : 해당 객체에 보낼 메세지
**************************************************************************/
void GameObject::SendCallbackMessage(const TagMessage message)
{
	//만약 딜레이 타임이 없다면 
	if (message.delayTime <= 0.0f)
	{ 
		//바로 해당 메세지의 이름과 같은 함수를 찾아서 실행시켜준다. 
		CallbackHashmapIter iter = _callbackList.find(message.name);
		if (iter != _callbackList.end())
		{
			iter->second(message);
		}
	}
	//만약 메세지의 딜레이 타임이 있다면
	else
	{
		//예약 메세지리스트에 넣어둔다. 
		//GameObject의 업데이트에서 예약 메세지의 시간을 재다가 시간이 다되었다면 호출
		_reserveMessageList.push_back(message);
	}
}
/*************************************************************************
## AddCallbackMessage ##
메세지 통신에 사용할 함수를 미리 등록한다. 
@@ string name : 등록할 함수의 이름
@@ function<void(TagMessage)> func : 등록할 함수 
**************************************************************************/
void GameObject::AddCallbackMessage(const string name,const function<void(struct TagMessage)> func)
{
	if (_callbackList.find(name) == _callbackList.end())
	{
		_callbackList.insert(make_pair(name, func));
	}
}

/*************************************************************************
## SetPivot ##
@@ Pivot::Enum pivot : 피봇
**************************************************************************/
void GameObject::SetPivot(const Pivot::Enum pivot)
{
	this->_pivot = pivot; 
	this->UpdateMainRect();
}
/*************************************************************************
## SetPosition ##
@@ Vector2 pos : 바꿔줄 좌표
**************************************************************************/
void GameObject::SetPosition(const Vector2 position)
{
	this->_position = position;
	this->UpdateMainRect();
}
/*************************************************************************
## SetSize ##
@@ Vector2 size : 바꿔줄 사이즈
**************************************************************************/
void GameObject::SetSize(const Vector2 size)
{
	this->_size = size; 
	this->UpdateMainRect();
}
/*************************************************************************
## SetActive ##
활성상태를 바꿔주고 활성,비활성될때에 실행되어야 할 함수를 호출
@@ bool b : 활성 여부 
**************************************************************************/
void GameObject::SetActive(const bool b)
{
	this->_isActive = b; 
	if (_isActive)
		this->Enable();
	else
		this->Disable();
}
void GameObject::SetMainRect(const RECT rect)
{
	this->_mainRect = rect;
}
/*************************************************************************
## UpdateMainRect ##
피봇에 따라서 렉트의 정보를 갱신해준다.
**************************************************************************/
void GameObject::UpdateMainRect()
{
	switch (_pivot)
	{
	case Pivot::LEFT_TOP:
		this->_mainRect = Figure::RectMake(_position, _size);
		break;
	case Pivot::CENTER:
		this->_mainRect = Figure::RectMakeCenter(_position, _size);
		break;
	case Pivot::BOTTOM:
		this->_mainRect = Figure::RectMakeBottom(_position, _size);
		break;
	default:
		break;
	}
}
