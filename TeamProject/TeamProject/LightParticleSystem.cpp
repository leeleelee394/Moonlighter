#include "stdafx.h"
#include "LightParticleSystem.h"

#include "LightParticle.h"
LightParticleSystem::LightParticleSystem(Vector2 pos)
{
	_position = pos;

	_currentTime  = 0.f;
	_durationTime = 1.f;
	_randomRange = 70.f;
	_currentIndex = 0;

	_mainRect = { -1000,-1000,-1000,-1000 };

	this->CreateParitcles();
}


LightParticleSystem::~LightParticleSystem()
{
	for (UINT i = 0; i < _particles.size(); ++i)
		SafeDelete(_particles[i]);
	_particles.clear();
}

void LightParticleSystem::Update()
{
	if (_LightingSystem->GetState() == LightSystem::State::Night)
	{
		_position = _ObjectManager->FindObject(ObjectType::Object, "Will")->GetPosition();
		_currentTime += _TimeManager->DeltaTime();
		if (_currentTime >= _durationTime)
		{
			while (_currentTime >= _durationTime)
				_currentTime -= _durationTime;

			Vector2 randomPos = Vector2(Math::Random(_position.x - _randomRange, _position.x + _randomRange),
				Math::Random(_position.y - _randomRange, _position.y + _randomRange));
			Vector2 randomDirection = Vector2(Math::Random(-1.f, 1.f), Math::Random(-1.f, 1.f));
			float randomLiveTime = Math::Random(3.f,6.f);
			_particles[_currentIndex]->Activation(randomPos, randomDirection, randomLiveTime);

			if (++_currentIndex >= _particles.size() - 1)
				_currentIndex = 0;
		}


		for (UINT i = 0; i < _particles.size(); ++i)
		{
			if (_particles[i]->GetActive())
			{
				_particles[i]->Update();
				_LightingSystem->RequestLighting(_particles[i]);
			}
		}
	}
}

void LightParticleSystem::Render()
{
	if (_isDebug)
	{
		if (_LightingSystem->GetState() == LightSystem::State::Night)
			_DXRenderer->DrawEllipse(_position, _randomRange, D2D1::ColorF::Blue, 1.f, true);

	}
}

void LightParticleSystem::CreateParitcles()
{
	for (UINT i = 0; i < 15; ++i)
	{
		this->_particles.push_back(new LightParticle());
	}
}

