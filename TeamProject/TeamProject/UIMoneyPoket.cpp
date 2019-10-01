#include "stdafx.h"
#include "UIMoneyPoket.h"
#include "Effect.h"


UIMoneyPoket::UIMoneyPoket()
{

}


UIMoneyPoket::~UIMoneyPoket()
{
}

void UIMoneyPoket::Init()
{
	//µ· ÁÖ¸Ó´Ï ÀÌ¹ÌÁö ·»´õ
	_moneyPoketIMG = _ImageManager->AddImage("moneyPoket", L"../Resources/UI/moneyBag.png", false);

}

void UIMoneyPoket::Release()
{
}

void UIMoneyPoket::Update()
{

}

void UIMoneyPoket::Render()
{
	//µ· ÁÖ¸Ó´Ï ÀÌ¹ÌÁö
	_moneyPoketIMG->SetSize(Vector2(64, 65));
	_moneyPoketIMG->Render(19, 18, Pivot::LEFT_TOP, false);
}
