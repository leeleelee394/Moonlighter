#pragma once
#include "GameObject.h"
class TitleDoor : public GameObject
{
	enum class State : int
	{
		Close = 0,OpenSlide0,OpenSlide1,OpenSlide2,Open
	};
private:
	State _state;
	Vector2 _direction;
	float _moveRange;
	float _delayTime;
	float _speed;
	class Image* _image;

	vector<class TitleDoorParticle*> _particles;
public:
	TitleDoor(string key,Vector2 direction,Vector2 pos,Vector2 size);
	~TitleDoor();

	void Update()override; 
	void Render()override; 

	void StateOpen() { _state = State::OpenSlide0; }
};

