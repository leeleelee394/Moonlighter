#pragma once
#include "PointLight.h"
class OilLampLight : public PointLight
{
public:
	OilLampLight(Vector2 pos, float range = 100.f, float intensity = 1.f, GameColor color = GameColor(1.f, 0.9f, 0.2f, 1.f));
	~OilLampLight();

	void Update()override;
	void Render()override;
};

