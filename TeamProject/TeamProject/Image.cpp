#include "stdafx.h"
#include "Image.h"

ID2D1RenderTarget* Image::renderTarget = nullptr;


/********************************************************************************
## Init ## 
프레임 없는 이미지 초기화
@@ ID2D1Bitmap* bitmap : 비트맵 포인터 (ResourceManager클래스에서 생성 후 넣어줌) 
@@ TagLoadedImageInfo info : 이미지 키값과 경로값을 담은 구조체
********************************************************************************/
void Image::Init(ID2D1Bitmap * bitmap, TagLoadedImageInfo loadinfo)
{
	this->bitmap = bitmap;
	this->loadInfo = loadinfo;

	this->size.x = (float)this->bitmap->GetPixelSize().width;
	this->size.y = (float)this->bitmap->GetPixelSize().height;

	this->scale.x = 1.0f;
	this->scale.y = 1.0f;
	this->alpha = 1.0f;
	this->angle = 0.0f;

	FrameRect rc;
	rc.X = 0;
	rc.Y = 0;
	rc.Width = size.x;
	rc.Height = size.y;
	this->frameInfo.push_back(rc);

	this->maxFrameX = this->maxFrameY = 1;
}

/********************************************************************************
## Init ## 
@@ ID2D1Bitmap* bitmap : 비트맵 포인터 (ResourceManager클래스에서 생성 후 인자로 넣어줌) 
@@ TagLoadImageInfo info : 이미지 키값과 경로값 담은 구조체 
@@ int maxFrameX : 가로 프레임 수 
@@ int maxFrameY : 세로 프레임 수 
********************************************************************************/
void Image::Init(ID2D1Bitmap * bitmap, TagLoadedImageInfo loadinfo, int maxframeX, int maxframeY)
{
	this->bitmap = bitmap;
	this->loadInfo = loadinfo;
	
	this->size.x = (float)bitmap->GetPixelSize().width;
	this->size.y = (float)bitmap->GetPixelSize().height;

	this->scale.x = 1.0f;
	this->scale.y = 1.0f;
	this->alpha = 1.0f;
	this->angle = 0.0f;

	this->maxFrameX = maxframeX;
	this->maxFrameY = maxframeY;

	float frameX = size.x / this->maxFrameX;
	float frameY = size.y / this->maxFrameY;

	FrameRect rc;
	for (int j = 0; j < maxFrameY; ++j)
	{
		for (int i = 0; i < maxFrameX; ++i)
		{
			rc.X = (float)i * (frameX);
			rc.Y = (float)j * (frameY);
			rc.Width = frameX;
			rc.Height = frameY;
			this->frameInfo.push_back(rc);
		}
	}
}

/********************************************************************************
## Release ## 
비트맵 해제
********************************************************************************/
void Image::Release()
{
	this->bitmap->Release();
	this->bitmap = nullptr;

	if (this->pixelCollisionDC)
		DeleteDC(this->pixelCollisionDC);
}

/********************************************************************************
## Render ## 
프레임 없는 이미지 렌더링
@@ int x : 랜더링 x좌표 
@@ int y : 랜더링 y좌표
@@ Pivot::Enum pivot : 렌더링 할 피봇 축
@@ bool isRelativePos : 카메라보정 유무
********************************************************************************/
void Image::Render(int x, int y, Pivot::Enum pivot, bool isRelativePos)
{
	size.x = size.x * scale.x;
	size.y = size.y * scale.y;

	Vector2 render = GetPivotPosition(x, y, pivot);

	if (isRelativePos)
	{
		//카메라
		float zoom = _Camera->GetZoom();
		Vector2 zoomScale = Vector2(zoom, zoom);
		size.x = size.x * zoomScale.x;
		size.y = size.y * zoomScale.y;

		//카메라 상대좌표
		render = _Camera->GetRelativeVector2(render);
	}
	//카메라에 없으면 랜더x
	RECT renderRc = Figure::RectMake((int)render.x, (int)render.y, (int)size.x, (int)size.y);
	if (renderRc.left > WinSizeX + 100 || renderRc.right < -100 ||
		renderRc.top > WinSizeY + 100 || renderRc.bottom < -100)
	{
		ResetRenderOption();
		return;
	}
	
	D2D1::Matrix3x2F reverse = D2D1::Matrix3x2F::Scale(scale.x, scale.y);
	Vector2 offset(0, 0);
	if (this->reverseAxisX)
	{
		reverse = D2D1::Matrix3x2F::Scale(-scale.x, scale.y);
		offset.x = size.x;
	}
	if (reverseAxisY)
	{
		reverse = D2D1::Matrix3x2F::Scale(scale.x, -scale.y);
		offset.y = size.y;
	}

	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(size.x / 2.f, size.y / 2.f));

	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, size.x, size.y);
	Image::renderTarget->SetTransform(reverse * rotation *
		SetupRender((int)(render.x + offset.x), (int)(render.y + offset.y)));
	Image::renderTarget->DrawBitmap(bitmap, dxArea, alpha);

	this->ResetRenderOption();

	_DXRenderer->AddDrawCall();
}

/********************************************************************************
## FrameRender ## 
@@ int x : 렌더링 할 x좌표
@@ int y : 렌더링 할 y좌표 
@@ int frameX : 가로 프레임
@@ int frameY : 세로 프레임 
@@ Pivot::Enum pivot : 렌더링 할 피봇		
@@ bool isRelativePos : 카메라 보정 유무 
********************************************************************************/
void Image::FrameRender(int x, int y, int frameX, int frameY, Pivot::Enum pivot, bool isRelativePos)
{
	if (frameX >= this->maxFrameX || frameY >= this->maxFrameY)
	{
		ResetRenderOption();
		return;
	}
	int frame = frameY * maxFrameX + frameX;
	//this->size = GetFrameSize(frame);

	size.x = size.x * scale.x;
	size.y = size.y * scale.y;

	Vector2 render = GetPivotPosition(x, y, pivot);

	if (isRelativePos)
	{
		//카메라
		float zoom = _Camera->GetZoom();
		Vector2 zoomScale = Vector2(zoom, zoom);
		size.x = size.x * zoomScale.x;
		size.y = size.y * zoomScale.y;

		//카메라 상대좌표
		render = _Camera->GetRelativeVector2(render);
	}
	//카메라에 없으면 랜더x
	RECT renderRc = Figure::RectMake((int)render.x, (int)render.y, (int)size.x, (int)size.y);
	if (renderRc.left > WinSizeX + 100 || renderRc.right < -100 ||
		renderRc.top > WinSizeY + 100 || renderRc.bottom < -100)
	{
		ResetRenderOption();
		return;
	}

	D2D1::Matrix3x2F reverse = D2D1::Matrix3x2F::Scale(scale.x, scale.y);
	Vector2 offset(0, 0);
	if (this->reverseAxisX)
	{
		reverse = D2D1::Matrix3x2F::Scale(-scale.x, scale.y);
		offset.x = size.x;
	}
	if (reverseAxisY)
	{
		reverse = D2D1::Matrix3x2F::Scale(scale.x, -scale.y);
		offset.y = size.y;
	}

	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(size.x / 2.f, size.y / 2.f));
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, size.x, size.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)frameInfo[frame].X,(float) frameInfo[frame].Y,
		(float)(frameInfo[frame].X + frameInfo[frame].Width),
		(float)(frameInfo[frame].Y + frameInfo[frame].Height));

	Image::renderTarget->SetTransform(reverse * rotation *
		SetupRender((int)(render.x + offset.x),(int)(render.y + offset.y)));
	Image::renderTarget->DrawBitmap(bitmap, dxArea, alpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	this->ResetRenderOption();

	_DXRenderer->AddDrawCall();
}

/********************************************************************************
## SetSize ## 
@@ Vector2 vec : 사이즈
********************************************************************************/
void Image::SetSize(Vector2 vec)
{
	this->size = vec;
}
/********************************************************************************
## SetAlpha ## 
@@ float alpha : 알파값
********************************************************************************/
void Image::SetAlpha(float alpha)
{
	this->alpha = alpha;
}
/********************************************************************************
## SetScale ##
@@ float scale : 스케일링 비율 
********************************************************************************/
void Image::SetScale(float scale)
{
	this->scale = Vector2(scale, scale);
}
/********************************************************************************
## SetAngle ## 
회전값 세팅 
@@ flaot angle : 디그리 값으로 넣어주자
********************************************************************************/
void Image::SetAngle(float angle /*디그리*/)
{
	this->angle = angle;
}
/********************************************************************************
## ResetRenderOption ## 
전체 렌더링 관련 값들 초기화 (렌더링 함수 호출 후에 자동으로 호출된다.)
********************************************************************************/
void Image::ResetRenderOption()
{
	this->alpha = 1.0f;
	this->scale.x = 1.0f;
	this->scale.y = 1.0f;
	this->size.x =(float) bitmap->GetPixelSize().width;
	this->size.y = (float)bitmap->GetPixelSize().height;
	this->reverseAxisX = this->reverseAxisY = false;
}
/********************************************************************************
## GetPixel ## 
********************************************************************************/

COLORREF Image::GetPixelColor(int x, int y)
{
	if (pixelCollisionDC != nullptr)
	{
		return GetPixel(pixelCollisionDC, x, y);
	}
	return RGB(0, 0, 0);
}
/********************************************************************************
## RenderPixel ##
********************************************************************************/
void Image::RenderPixel(HDC hdc, int x, int y)
{
	//비트블릿 == 백버퍼에 있는 걸 앞쪽으로 고속복사 해주는 함수
	BitBlt(hdc, x, x,
		size.x, size.y,
		pixelCollisionDC, 0, 0, SRCCOPY);
}

/********************************************************************************
## SetupRender ## 
@@ int x : 실질적으로 화면에 렌더링 될 좌표
@@ int y : 실질적으로 화면에 렌더링 될 좌표
********************************************************************************/
D2D1::Matrix3x2F Image::SetupRender(int x, int y)
{
	POINT pt = { (LONG)x,(LONG)y };
	this->renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	return D2D1::Matrix3x2F::Translation((float)pt.x,(float) pt.y);
}

/********************************************************************************
## GetPivotPosition ## 
@@ int x : 렌더링 할 x좌표
@@ int y : 렌더링 할 y좌표
@@ Pivot::Enum pivot : 렌더링 할 피봇
********************************************************************************/
Vector2 Image::GetPivotPosition(int x, int y, Pivot::Enum pivot)
{
	if (pivot == Pivot::LEFT_TOP) return Vector2(x, y);

	Vector2 pos = { x,y };

	switch (pivot)
	{
	case Pivot::CENTER:
		pos.x -= size.x * scale.x * 0.5f;
		pos.y -= size.y * scale.y * 0.5f;
		break;

	case Pivot::BOTTOM:
		pos.x -= size.x * scale.x * 0.5f;
		pos.y -= size.y * scale.y;
		break;
	}

	return pos;
}
