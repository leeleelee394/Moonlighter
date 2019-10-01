#include "stdafx.h"
#include "DXRenderer.h"

/********************************************************************************
## RenderText ## 
@@ int x : ������ �� x��ǥ 
@@ int y : ������ �� y��ǥ
@@ wstring text : ������ �� �ؽ�Ʈ 
@@ int size : ��Ʈ ������ 
@@ DefaultBrush::Enum brush : �⺻ �귯�� �÷� 
@@ DWRITE_TEXT_ALIGMENT align : �ؽ�Ʈ �׸� �Ǻ� ����
@@ wstring font : ��Ʈ (�⺻������ ���� ����) 
********************************************************************************/
void DXRenderer::RenderText(int x, int y, wstring text, int size, DefaultBrush::Enum defaultBrush, 
	DWRITE_TEXT_ALIGNMENT align,bool isRelative, wstring font /*=���� ����*/)
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
@@ int x : ������ �� x��ǥ 
@@ int y : ������ �� y��ǥ 
@@ COLORREF color: ������ �� �÷� 
@@ float alpha : �ؽ�Ʈ ���İ�
@@ int size : ��Ʈ ������
@@ align : �ؽ�Ʈ �׸� �Ǻ� ���� 
@@ wstring font : ��Ʈ ( �⺻������ ���� ���� ����) 
********************************************************************************/
void DXRenderer::RenderText(int x, int y, wstring text, COLORREF color, float alpha, int size,
	DWRITE_TEXT_ALIGNMENT align, bool isRelative, wstring font/*="��������"*/)
{
	Vector2 pos(x, y);
	if (isRelative)
	{
		pos = _Camera->GetRelativeVector2(pos);
	}

	//�ؽ�Ʈ ���̾ƿ� ����
	dwFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		dwTextFormat[font],
		(float)text.length() * size,
		(float)size,
		&dwLayout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize((float)size, range);
	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	//�귯�� ����
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

	//���̾ƿ� �¾�
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

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	dwLayout->SetFontSize((float)size, range);

	dwLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	dwLayout->SetTextAlignment(align);

	//�귯�� ����
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
@@ Vector2 start : ������ 
@@ Vector2 end : ����
@@ D2D1::ColorF::Enum color : �÷�
@@ flaot alpha : ���İ� 
@@ bool isRelativePos : ī�޶� ���� ���� ���� 
@@ flaot strokeWidth : �� ���� 
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
@@ Vector2 start : ������ 
@@ Vector2 end : ���� 
@@ DefaultBrush::Enum brush : �⺻ �귯�� ����
@@ bool isRelativePos : ī�޶� ���� ���� 
@@ flaot strokeWidth : ������ 
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
�簢�� �ܰ��� ������
@@ RECT rc : ��Ʈ ���� 
@@ D2D1::ColorF::Enum color : �÷� 
@@ float alpha : ���İ�
@@ bool isRelativePos : ī�޶� ���� ���� 
@@ float strokeWidth : �ܰ��� ���� 
********************************************************************************/
void DXRenderer::DrawRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
�簢�� �ܰ��� ������ 
@@ RECT rc : �簢�� ����
@@ DefaultBrush::Enum brush : �귯��
@@ bool isRelatviePos : ī�޶� ���� ����
@@ float strokeWidth : �� ���� 
********************************************************************************/
void DXRenderer::DrawRectangle(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
�� �ܰ��� ������ 
@@ RECT rc : �簢�� ���� 
@@ D2D1::ColorF::Enum color : Į�� 
@@ float alpha : ���İ� 0~1 
@@ bool isRelativePos : ī�޶� ���� ���� 
@@ float strokeWidth : �� ���� 
********************************************************************************/
void DXRenderer::DrawEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos, float strokeWidth)
{

	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
�� �ܰ��� ������
@@ RECT rc : �簢�� ����
@@ DefaultBrush::Enum brush : �귯��
@@ bool isRelativePos : ī�޶� ���� ���� 
@@ float strokeWidth : ������ 
********************************************************************************/
void DXRenderer::DrawEllipse(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos, float strokeWidth)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
	//ī�޶� ������ ����x
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
	//ī�޶� ������ ����x
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
@@ RECT rc : �簢�� ����
@@ D2D1::ColorF::Enum color : Į��
@@ float alpha : ���İ�
@@ bool isRelativePos : ī�޶� ���� ����
********************************************************************************/
void DXRenderer::FillRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
@@ RECT rc : �簢�� ���� 
@@ DefaultBrush::Enum brush : �귯�� 
@@ bool isRelatviePos : ī�޶� ���� ���� 
********************************************************************************/
void DXRenderer::FillRectangle(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
	//ī�޶� ������ ����x
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
@@ RECT rc : �簢�� ���� 
@@ D2D1::ColorF::Enum color : Į��
@@ float alpha : ���İ�
@@ bool isRelativePos : ī�޶� ���� ���� 
********************************************************************************/
void DXRenderer::FillEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
@@ RECT rc : �簢�� ����
@@ DEfaultBrush::Enum brush : �귯��
@@ bool isRelativePos : ī�޶� ���� ���� 
********************************************************************************/
void DXRenderer::FillEllipse(RECT rc, DefaultBrush::Enum defaultBrush, bool isRelativePos)
{
	if (isRelativePos)
	{
		rc = _Camera->GetRelativeRect(rc);
	}
	//ī�޶� ������ ����x
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
	//ī�޶� ������ ����x
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
	//ī�޶� ������ ����x
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