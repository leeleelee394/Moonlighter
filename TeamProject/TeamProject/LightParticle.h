#pragma once
#include "PointLight.h"
class LightParticle : public PointLight
{
private:
	float _activationTime;
	float _currentTime;
	Vector2 _direction;
public:
	LightParticle();
	~LightParticle();

	void Update()override; 

	void Activation(Vector2 pos,Vector2 direction, float liveTime);
};

