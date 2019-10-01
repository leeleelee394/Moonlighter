#include "stdafx.h"
#include "InterfaceClass.h"

#include "GameObject.h"
IEventCameraMove::IEventCameraMove(GameObject * object)
	:_target(object)
{
	
}


void IEventCameraMove::EventStart()
{
	_Camera->ChangeTarget(_target);
}

bool IEventCameraMove::EventUpdate()
{
	if (Math::GetDistance(_Camera->GetWorldViewportCenter().x, _Camera->GetWorldViewportCenter().y,
		_target->GetPosition().x, _target->GetPosition().y) <= CameraMoveStartDistance + 1.f)
	{
		return true;
	}
	return false;
}




IEventDelay::IEventDelay(float delayTime)
{
	_delayTime = delayTime;
	_currentTime = 0.f;
}

void IEventDelay::EventStart()
{

}

bool IEventDelay::EventUpdate()
{
	_currentTime += _TimeManager->DeltaTime();
	if (_currentTime >= _delayTime)
	{
		return true;
	}
	return false;
}
