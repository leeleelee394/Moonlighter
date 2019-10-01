#include "stdafx.h"
#include "EventManager.h"

#include "InterfaceClass.h"

EventManager* EventManager::_instance = nullptr;

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}

void EventManager::Update()
{
	if (_eventQueue.size() != 0)
	{
		if (_eventQueue[0]->EventUpdate() == true)
		{
			SafeDelete(_eventQueue[0]);
			_eventQueue.erase(_eventQueue.begin());
			if (_eventQueue.size() != 0)
			{
				_eventQueue[0]->EventStart();
			}
		}
	}
}

void EventManager::PushEvent(IEvent * pEvent)
{
	if (_eventQueue.size() == 0)
	{
		pEvent->EventStart();
	}
	this->_eventQueue.push_back(pEvent);

}
