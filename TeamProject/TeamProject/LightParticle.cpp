#include "stdafx.h"
#include "LightParticle.h"


LightParticle::LightParticle()
	:PointLight(Vector2(0,0),10.f,2.f)
{
}


LightParticle::~LightParticle()
{
}

void LightParticle::Update()
{
	float deltaTime = _TimeManager->DeltaTime();
	_position += _direction * 15.f * deltaTime;
	_currentTime += deltaTime;
	if (_currentTime - 2.f < _activationTime)
	{
		this->_intensity += deltaTime;
		if (_intensity >= 2.f)
			_intensity = 2.f;
	}
	else
	{
		this->_intensity -= deltaTime;
		if (_intensity <= 0.f)
			this->_isActive = false;
	}
}

void LightParticle::Activation(Vector2 pos,Vector2 direction, float liveTime)
{
	this->_isActive = true;
	this->_position = pos;
	this->_activationTime = liveTime;
	this->_direction = direction.Normalize();
	this->_currentTime = 0.f;
	this->_intensity = 0.f;
}

