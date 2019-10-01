#include "stdafx.h"
#include "UICoin.h"
#include "StringHelper.h"


UICoin::UICoin()
{
	//ÄÚÀÎ ÀÌ¹ÌÁö
	_coinIMG = _ImageManager->FindImage("Coin");
}


UICoin::~UICoin()
{
}

void UICoin::Init()
{
}

void UICoin::Release()
{
}

void UICoin::Update()
{
}

void UICoin::Render()
{
	//ÄÚÀÎ ÅØ½ºÆ® ·»´õ
	_DXRenderer->RenderText(40, 92, StringHelper::StringToWString(to_string(150)), RGB(255,255,255), 1.0f, 18, DWRITE_TEXT_ALIGNMENT_LEADING, false, L"¸¼Àº°íµñ");



	//ÄÚÀÎ ·»´õ
	_coinIMG->Render(78, 95, Pivot::LEFT_TOP, false);

	


}
