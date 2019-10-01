#pragma once
class TitleDoorParticle
{
private:
	float _frameCount; 
	float _startTime;
	bool _isPlay;
	Vector2 _pos;
	class Animation* _animation;
	class Image* _image; 
public:
	TitleDoorParticle(Vector2 pos,float startTime);
	~TitleDoorParticle();

	void Update();
	void Render(); 

	void Play() { _isPlay = true; }
	void MovePositionX(float moveValue);
};

