#pragma once
#include "GameObject.h"
class LightParticleSystem : public GameObject
{
private:
	int _currentIndex; 
	float _currentTime;
	float _durationTime;
	vector<class LightParticle*> _particles;

	float _randomRange;
public:
	LightParticleSystem(Vector2 pos);
	~LightParticleSystem();

	void Update()override; 
	void Render()override; 
private:
	void CreateParitcles();
};

