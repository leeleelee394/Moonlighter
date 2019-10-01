#pragma once
class Timer
{
private:
	float _frameCount;
public:
	Timer();
	~Timer();

	bool UpdateFrame(float frameRate);
};

