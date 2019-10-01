#include "stdafx.h"
#include "UIWeaponAndBag.h"
#include "Animation.h"

UIWeaponAndBag::UIWeaponAndBag()
{
	//상시 UI 무기 스왑
	this->_uIWeaponSwapIMG = _ImageManager->FindImage("UI_WeaponSwap");

	//상시 UI 가방
	this->_uIBagIMG = _ImageManager->FindImage("UI_Bag");

	//클래스 이름
	this->_name = "UIWeaponAndBag";
	this->_mainRect = Figure::RectMake(1057, 160, 40, 40);

	//애니메이션 new
	_uIswapAnimation = new Animation;

	//스왑 상태 초기화
	_isSwap = false;

	//시작 프레임 x,y와 끝 프레임 x,y
	_uIswapAnimation->SetStartEndFrame(0, 0, _uIWeaponSwapIMG->GetMaxFrameX() - 1,0, false);
	//루프 안함
	_uIswapAnimation->SetIsLoop(false);
	//프레임 도는 시간
	_uIswapAnimation->SetFrameUpdateTime(0.1f);

	//스왑 카운트 초기화
	_swapCount = 0;
}


UIWeaponAndBag::~UIWeaponAndBag()
{
}

void UIWeaponAndBag::Init()
{	
}

void UIWeaponAndBag::Release()
{

}

void UIWeaponAndBag::Update()
{
	//인벤토리
	//if (_Input->GetKeyDown(VK_F4))
	//{
	//	GameObject* object = _ObjectManager->FindObject(ObjectType::UI, "Inventory");
	//	if (object)
	//	{
	//		if (object->GetActive() == false)
	//		{
	//			object->SetActive(true);
	//		}
	//	}
	//}

	//무기 스왑 Z키
	if (_Input->GetKeyDown('Z'))
	{
		//스왑이 false인 경우
		//소드 -> 활 변환
		if (_isSwap == false) 
		{
			_uIswapAnimation->Stop();
			//시작 프레임 x,y와 끝 프레임 x,y
			_uIswapAnimation->SetStartEndFrame(0, 0, _uIWeaponSwapIMG->GetMaxFrameX() -1 , 0, false);

			//애니메이션 실행
			_uIswapAnimation->Play();

			//스왑 상태 변경
			_isSwap = true;
		}

		//스왑이 true인 경우
		//활 -> 소드 변환
		else if (_isSwap == true) 
		{
			_uIswapAnimation->Stop();
			//시작 프레임 x,y와 끝 프레임 x,y
			_uIswapAnimation->SetStartEndFrame(0, 1, _uIWeaponSwapIMG->GetMaxFrameX() - 1, 1, false);

			//애니메이션 실행
			_uIswapAnimation->Play();

			//스왑 상태 변경
			_isSwap = false;
		}
	}
	//프레임 업데이트
	_uIswapAnimation->UpdateFrame();

}

void UIWeaponAndBag::Render()
{
	//UI 무기 스왑 프레임 이미지 렌더
	if (_uIWeaponSwapIMG != nullptr) 
	{
		//SetSize에 uiWeaponSwapIMG의 프레임을 담는다.
		_uIWeaponSwapIMG->SetSize(_uIWeaponSwapIMG->GetFrameSize(0));


		_uIWeaponSwapIMG->FrameRender(922, 19, _uIswapAnimation->GetNowFrameX(), _uIswapAnimation->GetNowFrameY(), Pivot::LEFT_TOP, false);
	}

	//UI 가방 이미지 렌더
	if (_uIBagIMG != nullptr)
	{
		_uIBagIMG->Render(1018, 140, Pivot::LEFT_TOP, false);
	}

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::white, false, 2.0f);
	}
}
