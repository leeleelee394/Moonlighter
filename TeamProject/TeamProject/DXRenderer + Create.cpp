#include "stdafx.h"
#include "DXRenderer.h"

#include "Image.h"
/********************************************************************************
## CreateDevice ## 
D3D디바이스 및 DeviceContext 생성 
********************************************************************************/
void DXRenderer::CreateDevice()
{
	HRESULT hr;

	//클라이언트 영역 받아온다. 
	HWND handle = Window::GetWindowDesc().Handle;
	RECT rc;
	GetClientRect(handle, &rc);

	UINT width = (UINT)(rc.right - rc.left);
	UINT height = (UINT)(rc.bottom - rc.top);
	//디바이스 플래그 세팅
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//드라이버 타입 고사양부터 저사양으로 순차적 접근
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	//해당 컴이 지원하는 direct레벨 체크 
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	//해당 그래픽 드라이버 지원 레벨 체크 
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		//해당 드라이버 타입으로 디바이스 생성 시도 
		this->driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &d3dDevice, &this->featureLevel, &d3dDeviceContext);
		
		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &d3dDeviceContext);
		}
		//생성에 성공 했다면 해당 버젼으로 생성한 디바이스를 사용할 것이므로 빠져나가자 
		if (SUCCEEDED(hr))
			break;
	}
	//실패했으면 에러 메세지 
	assert(SUCCEEDED(hr));
	//팩토리 및 어뎁터 생성 부분 
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		//생성한 디바이스로 어뎁터 생성가능 한지 체킹 
		hr = d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			//디바이스 내의 어뎁터 가져온다. 
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				//어뎁터에서 팩토리를 받아온다. 
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	//실패했으면 에러 메세지 
	assert(SUCCEEDED(hr));

	// 스왑체인 생성 부분-------------------------------------------------------------------------------------------------------------------------------------
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		hr = d3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&d3dDevice1));
		if (SUCCEEDED(hr))
		{
			(void)d3dDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&d3dDevice1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(d3dDevice, Window::GetWindowDesc().Handle, &sd, nullptr, nullptr, &d3dSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = d3dSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&d3dSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = Window::GetWindowDesc().Handle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(d3dDevice, &sd, &d3dSwapChain);
		assert(SUCCEEDED(hr));
	}
	//-----------------------------------------------------------------------------------------------------------------------
	dxgiFactory->MakeWindowAssociation(Window::GetWindowDesc().Handle, DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();
}

/********************************************************************************
## CreateBackBuffer ## 
CreateDevice에서 생성한 스왑체인으로부터 RTV,DSV를 생성 
********************************************************************************/
void DXRenderer::CreateBackBuffer()
{
	//클라이언트 영역 받아옴
	HWND handle = Window::GetWindowDesc().Handle;
	RECT rc;
	GetClientRect(handle, &rc);

	UINT width = (UINT)(rc.right - rc.left);
	UINT height = (UINT)(rc.bottom - rc.top);

	HRESULT hr;
	//생성한 스왑체인으로 부터 백버퍼 정보를 받아온다, 
	hr = d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&d3dBackbufferTexture2D));
	assert(SUCCEEDED(hr));
	//렌더타겟뷰생성 
	hr = d3dDevice->CreateRenderTargetView(d3dBackbufferTexture2D, nullptr,
		&d3dRenderTargetView);

	//pBackBuffer->Release();
	assert(SUCCEEDED(hr));
	//렌더타겟용 텍스처 2d생성
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = d3dDevice->CreateTexture2D(&descDepth, nullptr, &d3dDepthStencilTexture);
	assert(SUCCEEDED(hr));

	//깊이 스텐실 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = d3dDevice->CreateDepthStencilView(d3dDepthStencilTexture, &descDSV, &d3dDepthStencilView);
	assert(SUCCEEDED(hr));

	d3dDeviceContext->OMSetRenderTargets(1, &d3dRenderTargetView, d3dDepthStencilView);
	//뷰포트 설정
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3dDeviceContext->RSSetViewports(1, &vp);
}
/********************************************************************************
## CreateDepthStencilState ##
DepthStencilState는 사용안함으로 설정
********************************************************************************/
void DXRenderer::CreateDepthStencilState()
{
	//스텐실 사용 안함 
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc = { 0 };
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = d3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &depthStencilDisableState);
	assert(SUCCEEDED(hr));

	d3dDeviceContext->OMSetDepthStencilState(depthStencilDisableState, 1);
}

/********************************************************************************
## CreateDirect2D ## 
D2D관련 멤버 변수들 생성
RenderTarget은 D3D스왑체인으로부터 Surface를 받아와서 생성 
********************************************************************************/
void DXRenderer::CreateDirect2D()
{
	HRESULT hr;
	//팩토리 생성 
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	assert(SUCCEEDED(hr));
	//스왑체인으로부터 백버퍼 정보 받아온다. 
	hr = d3dSwapChain->GetBuffer(0, IID_PPV_ARGS(&d2dBackBuffer));
	assert(SUCCEEDED(hr));
	FLOAT dpiX;
	FLOAT dpiY;
	//현재 컴퓨터의 dpi정보를 받아온다. 
	d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	//위에서 생성한 렌더타겟으로부터 정보를 받아옴
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
	//d2dRenderTarget생성 
	hr = d2dFactory->CreateDxgiSurfaceRenderTarget(d2dBackBuffer, &props, &d2dRenderTarget);

	assert(SUCCEEDED(hr));
	//DwriteFactory생성 
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwFactory));
	assert(SUCCEEDED(hr));

	//WIC 팩토리 생성
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	Image::SetRenderTarget(this->d2dRenderTarget);
}

/********************************************************************************
## CreateBrush ## 
기본 브러쉬 생성
********************************************************************************/
void DXRenderer::CreateBursh()
{
	if (d2dRenderTarget)
	{
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &dwDefaultBrush[DefaultBrush::black]);
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &dwDefaultBrush[DefaultBrush::white]);
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &dwDefaultBrush[DefaultBrush::yello]);
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 1.0f), &dwDefaultBrush[DefaultBrush::red]);
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 1.0f), &dwDefaultBrush[DefaultBrush::blue]);
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &dwDefaultBrush[DefaultBrush::green]);
		this->d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &dwDefaultBrush[DefaultBrush::gray]);
	}
}

/********************************************************************************
## AddTextFormat ## 
폰트 생성
@@ wstring font : 폰트 이름 
@@ float size : 사이즈 설정
********************************************************************************/
void DXRenderer::AddTextFormat(wstring font, float size /*=20*/)
{
	HRESULT hr;
	IDWriteTextFormat* format = NULL;
	//팩토리를 통해서 Text포맷 생성
	hr = this->dwFactory->CreateTextFormat(
		font.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		&format
	);
	assert(SUCCEEDED(hr));

	this->dwTextFormat.insert(make_pair(font, format));
}
