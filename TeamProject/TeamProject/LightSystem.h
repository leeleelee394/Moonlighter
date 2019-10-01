#pragma once
#include "ShaderBuffer.h"
#define _MaxPointLight 30

class SunLightBuffer : public ShaderBuffer
{
public:
	struct BufferData
	{
		GameColor sunColor;
		float sunIntensity;
		float worldAmbient;
		float padding[2];
	}data;

	SunLightBuffer()
		:ShaderBuffer(&data, sizeof BufferData)
	{
		data.sunColor = GameColor(1.f, 1.f, 1.f, 1.f);
		data.sunIntensity = 0.5f;
		data.worldAmbient = 0.5f;
	}

};

/*********************************************************
## LightManager ##
**********************************************************/
class LightSystem
{
public:
	enum class State : int
	{
		Default = 0,Afternoon ,Evening,Night
	};
private:
	typedef map<State, SunLightBuffer::BufferData>::iterator DataTableIter;
private:
	class RenderTargetBuffer* copyResource;
	class RenderTargetBuffer* renderTargetBuffer;

	class Shader* lightingShader;
	class Shader* deferredShader;

	class SunLightBuffer* sunBuffer;
	class PointLightBuffer* pointLightBuffer;

	vector<class PointLight*> pointLightList;

	State state;
	map<State, SunLightBuffer::BufferData> sunDataTable;
public:
	LightSystem();
	~LightSystem();

	void BeginLighting();
	void Lighting();
	void EndLighting();

	void OnGui();

	void RequestLighting(class PointLight* light);
	void ChangeState(State state);
	const State GetState()const { return this->state; }
private:
	void CreateTable();
};

/*********************************************************
## ShaderLightBuffer ##
**********************************************************/


class PointLightBuffer : public ShaderBuffer
{
public:
	struct PointLightData
	{
		GameColor lightColor;
		Vector2 origin;
		float originRange;
		float intensity;
	};

	struct BufferData
	{
		UINT lightSize;
		float padding[3];
		PointLightData lightList[_MaxPointLight];
	}data;

	PointLightBuffer()
		:ShaderBuffer(&data, sizeof BufferData)
	{
		data.lightSize = 0;
	}
};
