#pragma once
#include "GameObject.h" 
class DungeonGate : public GameObject
{
public:
	enum class State : int
	{
		Idle = 0, Opening, OpenIdle, Closing, Enter, Exit, End
	};
private:
	struct TagRenderInfo
	{
		class Animation* animation;
		class Image* image; 
	};

	State _state;

	map<State, TagRenderInfo> _renderInfoList; 
	typedef map<State, TagRenderInfo>::iterator RenderInfoIter;

	class Image* _mainImage;
	class Animation* _mainAnimaition;
	class GameObject* _player;

	RECT _collider;
public:
	DungeonGate();
	~DungeonGate();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 

	RECT GetCollisionRect()const override { return _collider; }

public:
	void ChangeState(State state);
	void ChangeRenderInfo(State state);
};

