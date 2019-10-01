#include "stdafx.h"
#include "BossEffect.h"
#include "Animation.h"


BossEffect::BossEffect()
{
	//보스 스크롤 이미지
	_bossScrollIMG = _ImageManager->FindImage("bossScrollEffect");
	
	//보스 스크롤 애니메이션 new 선언
	_bossScrollAnimation = new Animation;

	//시작 프레임 x,y와 끝 프레임 x,y
	_bossScrollAnimation->SetStartEndFrame(0, 0, _bossScrollIMG->GetMaxFrameX() - 1, 0, false);

	//루프 안함
	_bossScrollAnimation->SetIsLoop(false);

	//프레임 도는 시간
	_bossScrollAnimation->SetFrameUpdateTime(0.15f);
	//프레임 시작
	_bossScrollAnimation->Play();

	//알파 값 변수 초기화
	_alpha = 1.0f;
}


BossEffect::~BossEffect()
{
}

void BossEffect::Init()
{
}

void BossEffect::Release()
{
}

void BossEffect::Update()
{

	//애니메이션 업데이트 프레임
	_bossScrollAnimation->UpdateFrame();
}

void BossEffect::Render()
{
	//사이즈
	_bossScrollIMG->SetSize(_bossScrollIMG->GetFrameSize(0));
	//프레임 렌더
	//_bossScrollIMG->FrameRender(929, 929, _bossScrollAnimation->GetNowFrameX(), 0, Pivot::CENTER, false);
	_bossScrollIMG->FrameRender(WinSizeX/2, WinSizeY/2 + 130, _bossScrollAnimation->GetNowFrameX(), 0, Pivot::CENTER, false);
	
	//스크롤 애니메이션이 16 프레임이면 알파 값 적용
	if (_bossScrollAnimation->GetNowFrameX() == 16) 
	{
		//알파 값 -0.01씩 낮추기
		_alpha -= 0.01f;

		//0보다 낮아지면 0으로 계속 초기화
		if (_alpha <= 0.f) 
		{
			_alpha = 0;
		}
	}
	//알파 값 세팅
	_bossScrollIMG->SetAlpha(_alpha);

	//스케일 세팅
	_bossScrollIMG->SetScale(1.5f);
}
