#pragma once
class EventManager
{
public:
	static EventManager* Get()
	{
		if (_instance == nullptr)
			_instance = new EventManager;
		return _instance;
	}
private:
	static EventManager* _instance; 
private:
	vector<class IEvent*> _eventQueue;
public:
	EventManager();
	~EventManager();

	void Update();

	void PushEvent(class IEvent* pEvent);
};

#define _EventManager EventManager::Get()

