#pragma once

#include <unordered_map>

namespace DefaultBrush
{
	enum Enum
	{
		white, black, yello, red, blue, green, gray, end
	};
}

class DXRenderer
{
	SingletonHeader(DXRenderer)
private:
	typedef unordered_map<wstring, IDWriteTextFormat*> MapTextList;
	typedef unordered_map<wstring, IDWriteTextFormat*>::iterator MapTextIter;
private:
	//DirectX11 ----------------------------------------
	//그래픽카드 드라이버 타입
	D3D_DRIVER_TYPE driverType;
	//direct 지원 레벨 검사해줄 피쳐레벨
	D3D_FEATURE_LEVEL featureLevel; 
	
	ID3D11Device* d3dDevice = nullptr;
	ID3D11Device1* d3dDevice1 = nullptr;

	ID3D11DeviceContext* d3dDeviceContext = nullptr;
	ID3D11DeviceContext1* d3dDeviceContext1 = nullptr;

	IDXGISwapChain* d3dSwapChain = nullptr;
	IDXGISwapChain1* d3dSwapChain1 = nullptr;

	ID3D11RenderTargetView* d3dRenderTargetView = nullptr;
	ID3D11Texture2D* d3dDepthStencilTexture = nullptr;
	ID3D11DepthStencilView* d3dDepthStencilView = nullptr;
	ID3D11DepthStencilState* depthStencilDisableState = nullptr;
	ID3D11Texture2D* d3dBackbufferTexture2D = nullptr;
	//Direct2d----------------------------------------------
	//다이렉트2D (렌더링 엔진)
	ID2D1Factory*						d2dFactory = nullptr;
	ID2D1RenderTarget*					d2dRenderTarget = nullptr;
	IDXGISurface*						d2dBackBuffer = nullptr;

	//WIC (이미지 처리 컴포넌트)
	IWICImagingFactory*					wicFactory = nullptr;
	IWICFormatConverter*				convertedSrcBmp = nullptr;

	//다이렉트 Write (텍스트 엔진)
	IDWriteFactory*						dwFactory = nullptr;
	IDWriteTextLayout*					dwLayout = nullptr;

	ID2D1SolidColorBrush*				dwDefaultBrush[DefaultBrush::end];
	MapTextList							dwTextFormat;

	UINT								prevDrawcall;
	UINT								currentDrawCall;
private:
	void CreateDevice();
	void CreateBackBuffer();
	void CreateDepthStencilState();
	void CreateDirect2D();
	void CreateBursh();
	void ReleaseDevice();

	void AddTextFormat(wstring font, float size);
public:
	UINT GetDrawCall()const { return this->prevDrawcall; }
	void AddDrawCall(){ ++this->currentDrawCall; }
public:
	void BindSwapchainRenderTargetAndClear();
	void BindSwapChainRenderTarget();
	void PresentSwapChain();
	void Direct2DBeginDraw();
	void Direct2DEndDraw();
public:
	//텍스트 출력(기본 브러쉬 사용) 
	void RenderText(int x, int y, wstring text, int size,
		DefaultBrush::Enum defaultBrush = DefaultBrush::black,
		DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING, bool isRelative = false,
		wstring font = L"맑은고딕");
	//텍스트 출력 (원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void RenderText(int x, int y, wstring text, COLORREF color, float alpha, int size,
		DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING, bool isRelative = false,
		wstring font = L"맑은고딕");
	//텍스트 필드 출력 (기본 브러쉬 사용) 
	void RenderTextField(int x, int y, wstring text, int size, int width, int height,
		DefaultBrush::Enum defaultBrush = DefaultBrush::black,
		DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING, bool isRelative = false,
		wstring font = L"맑은고딕");
	//텍스트 필드 출려(원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void RenderTextField(int x, int y, wstring text, COLORREF color, int size, int width, int height,
		float alpha = 1.0f,
		DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING, bool isRelative = false,
		wstring font = L"맑은고딕");

	void DrawLine( Vector2 start, Vector2 end, D2D1::ColorF::Enum color, float alpha, bool isRelativePos = false, float strokeWidth = 1);
	void DrawLine(Vector2 start, Vector2 end, DefaultBrush::Enum defaultBrush = DefaultBrush::black, bool isRelativePos = false, float strokeWidth = 1);
	void DrawRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos = false, float strokeWidth = 1);
	void DrawRectangle(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black, bool isRelativePos = false, float strokeWidth = 1);
	void DrawEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos = false, float strokeWidth = 1);
	void DrawEllipse(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black, bool isRelativePos = false, float strokeWidth = 1);
	void DrawEllipse(Vector2 origin, float radius, DefaultBrush::Enum defaultBrush = DefaultBrush::black, bool isARelativePos = false, float strokeWidth = 1);
	void DrawEllipse(Vector2 origin,float radius, D2D1::ColorF::Enum color, float alpha, bool isRelativePos = false, float strokeWidth = 1);
	void FillRectangle(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos = false);
	void FillRectangle(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black, bool isRelativePos = false);
	void FillRectangle(RECT rc,COLORREF color, float alpha, bool isRelativePos = false);
	void FillEllipse(RECT rc, D2D1::ColorF::Enum color, float alpha, bool isRelativePos = false);
	void FillEllipse(RECT rc, DefaultBrush::Enum defaultBrush = DefaultBrush::black, bool isRelativePos = false);
	void FiilEllipse(Vector2 origin,float radius, D2D1::ColorF::Enum color, float alpha, bool isRelative = false);
	void FiilEllipse(Vector2 origin,float radius, DefaultBrush::Enum brush = DefaultBrush::black, bool isRelativePos = false);
public:
	ID3D11Device* GetD3DDevice()const { return this->d3dDevice; }
	ID3D11DeviceContext* GetD3DDeviceContext()const { return this->d3dDeviceContext; }
	ID2D1RenderTarget* GetD2DRenderTarget()const { return this->d2dRenderTarget; }
	ID2D1Factory* GetD2DFactory()const { return this->d2dFactory; }
	IWICImagingFactory* GetImageingFactory()const { return this->wicFactory; }
	IWICFormatConverter* GetFormatConverter()const { return this->convertedSrcBmp; }
	ID3D11Texture2D* GetBackBufferTexture2d()const { return this->d3dBackbufferTexture2D; }
};

#define _DXRenderer DXRenderer::Get()
#define _D3DDevice DXRenderer::Get()->GetD3DDevice()
#define _D3DDeviceContext DXRenderer::Get()->GetD3DDeviceContext()
#define _D2DRenderTarget DXRenderer::Get()->GetD2DRenderTarget()