#include "stdafx.h"
#include "PointLight.h"

#include "LightSystem.h"

PointLight::PointLight(Vector2 pos, float range, float intensity, GameColor color)
	:GameObject("PointLight",pos,Vector2(),Pivot::CENTER),_originRange(range),_intensity(intensity),_color(color)
{
	_isIncrease = true;
	_increaseCount = 0.f;
}

PointLight::~PointLight()
{
}

void PointLight::Update()
{
	_LightingSystem->RequestLighting(this);
}

void PointLight::Render()
{
	//if (_isDebug)
	//{
	//	ImGui::Begin("PointLight");
	//	{
	//		ImGui::SliderFloat2("Origin", &_position.x, 0.f, CastingFloat(WinSizeX * 3));
	//		ImGui::SliderFloat("OriginRange", &_originRange, 0.f, 500.0f);
	//		ImGui::SliderFloat("Intensity", &_intensity, 0.f, 10.0f);
	//		ImGui::ColorEdit4("Color", &_color.r,
	//			ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreviewHalf);
	//	}
	//	ImGui::End();
	//}
}
