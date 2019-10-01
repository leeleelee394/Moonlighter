#include "stdafx.h"
#include "DXRenderer.h"

/********************************************************************************
## RenderText ## 
@@ int x : 렌더링 할 x좌표 
@@ int y : 렌더링 할 y좌표
@@ wstring text : 렌더링 할 텍스트 
@@ int size : 폰트 사이즈 
@@ DefaultBrush::Enum brush : 기본 브러쉬 컬러 
@@ DWRITE_TEXT_ALIGMENT align : 텍스트 그릴 피봇 설정
@@ wstring font : 폰트 (기본값으로 맑은 고딕) 
********************************************************************************/
void DXRenderer::RenderText(int x, int y, wstring text, int size, DefaultBrush::Enum defaultBrush, 
	DWRITE_TEXT_ALIGNMENT align,bool isRelative, wstring font /*=맑은 고딕*/)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}

	this->dwFactory->CreateTextLayout
	(
		text.c_str(),
		text.length(),
		dwTextFormat[font],
		(float)text.length() * size,
		(float)size,
		&dwLayout
	);

	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();

	dwLayout->SetFontSize((float)size, range);
	dwLayout->SetTextAlignment(align);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwLayout,
		dwDefaultBrush[defaultBrush]);

	dwLayout->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## RenderText ## 
@@ int x : 렌더링 할 x좌표 
@@ int y : 렌더링 할 y좌표 
@@ COLORREF color: 렌더링 할 컬러 
@@ float alpha : 텍스트 알파값
@@ int size : 폰트 사이즈
@@ align : 텍스트 그릴 피봇 설정 
@@ wstring font : 폰트 ( 기본값으로 맑은 고딕 설정) 
********************************************************************************/
void DXRenderer::RenderText(int x, int y, wstring text, COLORREF color, float alpha, int size,
	DWRITE_TEXT_ALIGNMENT align, bool isRelative, wstring font/*="맑은고딕"*/)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}

	//텍스트 레이아웃 생성
	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormat[font],
		(float)text.length() * size,
		(float)size,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize((float)size, range);
	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	//브러쉬 생성
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwLayout, brush);

	brush->Release();
	dwLayout->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## RenderTextField ##
********************************************************************************/
void DXRenderer::RenderTextField(int x, int y, wstring text, int size, int width, int height, 
	DefaultBrush::Enum defaultBrush, DWRITE_TEXT_ALIGNMENT align, bool isRelative, wstring font)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}

	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormat[font],
		(float)width,
		(float)height,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize((float)size, range);

	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), dwLayout, dwDefaultBrush[defaultBrush]);

	dwLayout->Release();
	++this->currentDrawCall;
}

void DXRenderer::RenderTextField(int x, int y, wstring text, COLORREF color,
	int size, int width, int height, float alpha, DWRITE_TEXT_ALIGNMENT align, bool isRelative, wstring font)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}


	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormat[font],
		(float)width,
		(float)height,
		&dwLayout
	);

	//레이아웃 셋업
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize((float)size, range);

	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	//브러쉬 생성
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawTextLayout(D2D1::Point2F((float)x, (float)y), dwLayout, brush);

	brush->Release();
	dwLayout->Release();
	++this->currentDrawCall;
}

/********************************************************************************
## DrawLine ## 
@@ Vector2 start : 시작점 
@@ Vector2 end : 끝점
@@ D2D1::ColorF::Enum color : 컬러
@@ flaot alpha : 알파값 
@@ bool isRelativePos : 카메라 기준 보정 유무 
@@ flaot strokeWidth : 선 굵기 
********************************************************************************/
void DXRenderer::DrawLine( Vector2 start, Vector2 end, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	if (isRelativePos)
	{
		start = _Camera->GetRelativeVector2(start);
		end = _Camera->GetRelativeVector2(end);
	}


	d2dRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), brush, strokeWidth);

	brush->Release();
	++this->currentDrawCall;
}

/********************************************************************************
## DrawLine ## 
@@ Vector2 start : 시작점 
@@ Vector2 end : 끝점 
@@ DefaultBrush::Enum brush : 기본 브러쉬 색상
@@ bool isRelativePos : 카메라 보정 유무 
@@ flaot strokeWidth : 선굵기 
********************************************************************************/
void DXRenderer::DrawLine(Vector2 start, Vector2 end, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	if (isRelativePos)
	{
		start = _Camera->GetRelativeVector2(start);
		end = _Camera->GetRelativeVector2(end);
	}

	d2dRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y),
		D2D1::Point2F(end.x, end.y), dwDefaultBrush[defaultBrush], strokeWidth);

	++this->currentDrawCall;
}

/********************************************************************************
## DrawRectangle ## 
사각형 외곽선 렌더링
@@ RECT rc : 렉트 영역 
@@ D2D1::ColorF::Enum color : 컬러 
@@ float alpha : 알파값
@@ bool isRelativePos : 카메라 보정 유무 
@@ float strokeWidth : 외곽선 굵기 
********************************************************************************/
void DXRenderer::DrawRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dRenderTarget->DrawRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		brush, strokeWidth);

	brush->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## DrawRectangle ## 
사각형 외곽선 렌더링 
@@ RECT rc : 사각형 영역
@@ DefaultBrush::Enum brush : 브러쉬
@@ bool isRelatviePos : 카메라 보정 유무
@@ float strokeWidth : 선 굵기 
********************************************************************************/
void DXRenderer::DrawRectangle(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dRenderTarget->DrawRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		dwDefaultBrush[defaultBrush], strokeWidth);

	++this->currentDrawCall;
}

/********************************************************************************
## DrawEllipse ## 
원 외곽선 렌더링 
@@ RECT rc : 사각형 영역 
@@ D2D1::ColorF::Enum color : 칼라 
@@ float alpha : 알파값 0~1 
@@ bool isRelativePos : 카메라 보정 유무 
@@ float strokeWidth : 선 굵기 
********************************************************************************/
void DXRenderer::DrawEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{

	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5f;
	ellipse.point.y = rc.top + height * 0.5f;
	ellipse.radiusX = width * 0.5f;
	ellipse.radiusY = height * 0.5f;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	brush->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## DrawEllipse ## 
원 외곽선 렌더링
@@ RECT rc : 사각형 영역
@@ DefaultBrush::Enum brush : 브러쉬
@@ bool isRelativePos : 카메라 보정 유무 
@@ float strokeWidth : 선굵기 
********************************************************************************/
void DXRenderer::DrawEllipse(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5f;
	ellipse.point.y = rc.top + height * 0.5f;
	ellipse.radiusX = width * 0.5f;
	ellipse.radiusY = height * 0.5f;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawEllipse(&ellipse, dwDefaultBrush[defaultBrush], strokeWidth);

	++this->currentDrawCall;
}

void DXRenderer::DrawEllipse(Vector2 origin, float radius, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	RECT rc = Figure::RectMakeByEllipse(origin, radius);
	Vector2 pos = origin;
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawEllipse(&ellipse, dwDefaultBrush[defaultBrush], strokeWidth);

	++this->currentDrawCall;
}

void DXRenderer::DrawEllipse(Vector2 origin, float radius, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	RECT rc = Figure::RectMakeByEllipse(origin, radius);
	Vector2 pos = origin;
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	brush->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## FillRectangle ## 
@@ RECT rc : 사각형 영역
@@ D2D1::ColorF::Enum color : 칼라값
@@ float alpha : 알파값
@@ bool isRelativePos : 카메라 보정 유무
********************************************************************************/
void DXRenderer::FillRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


	d2dRenderTarget->FillRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom), brush);

	brush->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## FillRectangle ## 
@@ RECT rc : 사각형 영역 
@@ DefaultBrush::Enum brush : 브러쉬 
@@ bool isRelatviePos : 카메라 보정 유무 
********************************************************************************/
void DXRenderer::FillRectangle(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dRenderTarget->FillRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		dwDefaultBrush[defaultBrush]);

	++this->currentDrawCall;
}

void DXRenderer::FillRectangle(RECT rc, COLORREF color, float alpha, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}
	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	d2dRenderTarget->FillRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		brush);

	brush->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## FillEllipse ## 
@@ RECT rc : 사각형 영역 
@@ D2D1::ColorF::Enum color : 칼라값
@@ float alpha : 알파값
@@ bool isRelativePos : 카메라 보정 유무 
********************************************************************************/
void DXRenderer::FillEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5f;
	ellipse.point.y = rc.top + height * 0.5f;
	ellipse.radiusX = width * 0.5f;
	ellipse.radiusY = height * 0.5f;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->FillEllipse(&ellipse, brush);

	brush->Release();

	++this->currentDrawCall;
}

/********************************************************************************
## FillEllipse ## 
@@ RECT rc : 사각형 영역
@@ DEfaultBrush::Enum brush : 브러쉬
@@ bool isRelativePos : 카메라 보정 여부 
********************************************************************************/
void DXRenderer::FillEllipse(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = rc.left + width * 0.5f;
	ellipse.point.y = rc.top + height * 0.5f;
	ellipse.radiusX = width * 0.5f;
	ellipse.radiusY = height * 0.5f;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->FillEllipse(&ellipse, dwDefaultBrush[defaultBrush]);

	++this->currentDrawCall;
}

void DXRenderer::FiilEllipse(Vector2 origin,float radius, D2D1::ColorF::Enum color, float alpha, bool isRelative)
{
	RECT rc = Figure::RectMakeByEllipse(origin, radius);
	Vector2 pos = origin;
	if (isRelative)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	ID2D1SolidColorBrush* brush;
	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->FillEllipse(&ellipse, brush);

	brush->Release();

	++this->currentDrawCall;
}

void DXRenderer::FiilEllipse(Vector2 origin, float radius, DefaultBrush::Enum brush, bool isRelativePos)
{
	RECT rc = Figure::RectMakeByEllipse(origin, radius);
	Vector2 pos = origin;
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
		pos = _Camera->GetRelativeVector2(pos);
	}
	//카메라에 없으면 랜더x
	if (rc.left > WinSizeX || rc.right < 0 ||
		rc.top  > WinSizeY || rc.bottom < 0)
	{
		return;
	}

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	d2dRenderTarget->FillEllipse(&ellipse, dwDefaultBrush[brush]);

	++this->currentDrawCall;
}
