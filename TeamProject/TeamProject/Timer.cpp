#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
	:_frameCount(0.f)
{
}


Timer::~Timer()
{
}

bool Timer::UpdateFrame(float frameRate)
{
	_frameCount += _TimeManager->DeltaTime();
	if (_frameCount >= frameRate)
	{
		while (_frameCount >= frameRate)
		{
			_frameCount -= frameRate;
		}
		return true;
	}
	return false;
}
