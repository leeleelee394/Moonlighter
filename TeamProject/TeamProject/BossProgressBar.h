#pragma once
#include "GameObject.h"
class BossProgressBar :	public GameObject
{
enum class BossProgressState
{
	DamageState = 0,
	DamageEndState,
	DefaultState,
	End
};
private:
	class Image* _bossProgressBarImg;		//보스 프로그래스 바 이미지
	BossProgressState _bossProgressState;	//보스 프로그래스 바 상태

	//HP바 위치 좌표 X, Y
	Vector2 _hpPosition;					//hp 위치 좌표 X, Y

	//이미지 프레임
	int _imgFrameX;							//이미지 프레임 X
	int _imgFrameY;							//이미지 프레임 Y

	float _frameCount;						//프레임 카운트

	int _hpWidth;							//체력바 길이

	float _saveHp;							//세이브 hp


public:
	BossProgressBar();
	~BossProgressBar();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	//게이지 얻기
	bool SetGauge(int currentHp, int maxHp);

	//보스 프로그래스 바 상태
	void BossProgressBarState();
};

