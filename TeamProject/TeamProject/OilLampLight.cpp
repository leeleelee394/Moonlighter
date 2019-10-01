#include "stdafx.h"
#include "OilLampLight.h"


OilLampLight::OilLampLight(Vector2 pos, float range, float intensity, GameColor color)
	:PointLight(pos,range,intensity,color)
{
}


OilLampLight::~OilLampLight()
{
}

void OilLampLight::Update()
{
	if (_LightingSystem->GetState() == LightSystem::State::Night ||
		_LightingSystem->GetState() == LightSystem::State::Default)
	{
		float deltaTime = _TimeManager->DeltaTime();
		if (_isIncrease)
		{
			_originRange += 10.f * deltaTime;
			_increaseCount += deltaTime;
			if (_increaseCount >= 2.f)
			{
				_isIncrease = !_isIncrease;
				_increaseCount = 0.f;
			}
		}
		else
		{
			_originRange -= 10.f * deltaTime;
			_increaseCount += deltaTime;
			if (_increaseCount >= 2.f)
			{
				_isIncrease = !_isIncrease;
				_increaseCount = 0.f;
			}
		}
		_LightingSystem->RequestLighting(this);
	}
}

void OilLampLight::Render()
{
	PointLight::Render();
}
