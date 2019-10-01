#pragma once

class IEvent
{
public:
	virtual void EventStart() = 0;
	virtual bool EventUpdate() = 0;
};

class IEventCameraMove : public IEvent
{
private:
	class GameObject* _target;
public:
	void EventStart()override;
	bool EventUpdate()override; 

	IEventCameraMove(class GameObject* object);
};

class IEventDelay : public IEvent
{
	float _currentTime;
	float _delayTime;
public:

	IEventDelay(float delayTime);
	void EventStart()override; 
	bool EventUpdate()override; 
};

