#include "stdafx.h"
#include "DXRenderer.h"

SingletonCpp(DXRenderer)

/********************************************************************************
## DXRenderer ## 
D3D디바이스 생성 -> DSV,RTV 생성 -> D2D렌더타겟,팩토리 등등 생성 -> 기본 브러쉬 생성 
	-> 기본 폰트등록 
********************************************************************************/
DXRenderer::DXRenderer()
	:currentDrawCall(0),prevDrawcall(0)
{
	this->CreateDevice();
	this->CreateBackBuffer();
	this->CreateDirect2D();
	this->CreateBursh();
	this->AddTextFormat(L"맑은고딕", 1);
	this->AddTextFormat(L"Cooper Std", 1);
}
/********************************************************************************
## ~DXRenderer ## 
모든 Direct관련 멤버변수들 해제 
********************************************************************************/
DXRenderer::~DXRenderer()
{
	this->ReleaseDevice();
}

/********************************************************************************
## ReleasesDevice ## 
모든 Direct관련 멤버 변수들 해제 
********************************************************************************/
void DXRenderer::ReleaseDevice()
{
	for (int i = 0; i < DefaultBrush::end; ++i)
		this->dwDefaultBrush[i]->Release();

	SafeRelease(d2dFactory);
	SafeRelease(d2dRenderTarget);
	SafeRelease(d2dBackBuffer);

	SafeRelease(wicFactory);
	SafeRelease(convertedSrcBmp);

	//SafeRelease(dwLayout);
	SafeRelease(dwFactory);

	SafeRelease(depthStencilDisableState);

	SafeRelease(d3dRenderTargetView);
	SafeRelease(d3dDepthStencilTexture);
	SafeRelease(d3dDepthStencilView);
	SafeRelease(d3dBackbufferTexture2D);

	SafeRelease(d3dSwapChain);
	SafeRelease(d3dSwapChain1);

	SafeRelease(d3dDeviceContext);
	SafeRelease(d3dDeviceContext1);

	SafeRelease(d3dDevice);
	SafeRelease(d3dDevice1);
}

/********************************************************************************
## BindSwapChainRenderTarget ## 
스왑체인으로 부터 생성한 rtv와 dsv를 초기화 한 후 파이프라인에 바인딩 d
********************************************************************************/
void DXRenderer::BindSwapchainRenderTargetAndClear()
{
	//RenderTargetView Clear해준다.
	float color[4] = { 0.3f,0.3f,0.3f,1.0f };
	d3dDeviceContext->ClearRenderTargetView(d3dRenderTargetView, color);
	//깊이버퍼도 밀어준다. 
	d3dDeviceContext->ClearDepthStencilView(d3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//깊이 버퍼와 렌더타겟 뷰 바인딩
	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, nullptr);
}
/**********************************************************************************
## BindWapChainRenderTarget ##

***********************************************************************************/
void DXRenderer::BindSwapChainRenderTarget()
{
	//RenderTargetView Clear해준다.
	float color[4] = { 0.3f,0.3f,0.3f,1.0f };
	d3dDeviceContext->ClearRenderTargetView(d3dRenderTargetView, color);
	//깊이 버퍼와 렌더타겟 뷰 바인딩
	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, nullptr);
}
/********************************************************************************
## PresentSwapChain ## 
D3D 더블버퍼링 교환 호출  
********************************************************************************/
void DXRenderer::PresentSwapChain()
{
	d3dSwapChain->Present(0, 0);
}

/********************************************************************************
## Direct2DBeginDraw ## 
D2D렌더링 시작 
********************************************************************************/
void DXRenderer::Direct2DBeginDraw()
{
	d2dRenderTarget->BeginDraw();
}

/********************************************************************************
##Direct2DEndDraw ##
D2D렌더링 종료 및 현재 프레임 드로우 콜 수 리셋
********************************************************************************/
void DXRenderer::Direct2DEndDraw()
{
	//TODO DevieLost상황에 대비 추가 해야함
	d2dRenderTarget->EndDraw();

	prevDrawcall = currentDrawCall;
	currentDrawCall = 0;
}


