#pragma once
#include "GameObject.h"
class ProgressBar :	public GameObject
{
	enum class ProgressState 
	{
		DamageState = 0,
		DamageEndState,
		DefaultState,
		End
	};

private:
	
	//이미지 클래스 선언 - 프로그래스바
	class Image* _progressBarBackIMG;
	class Image* _progressBarFrontIMG;

	//이미지 클래스 선언 - 하트
	class Image* _heartIMG;

	//HP바 위치 좌표 X, Y
	Vector2 _hpPosition;

	//이미지 프레임
	int _imgFrameX;						//이미지 프레임 X
	int _imgFrameY;						//이미지 프레임 Y

	float _frameCount;					//프레임 카운트

	//체력바 길이
	int _hpWidth;
	
	//세이브 HP
	float _saveHp;

	ProgressState _barState;			//프로그래스 바 상태

	//테스트용
	//현재 HP
	int _testCurrentHp;

	//최대 HP
	int _testMaxHp;





public:
	ProgressBar();
	~ProgressBar();

	void Init();
	void Release();
	void Update();
	void Render();

	//게이지 얻기
	bool SetGauge(int currentHp, int maxHp);

	//프로그래스 바 상태
	void ProgressBarState();
};

