#include "stdafx.h"
#include "ImageManager.h"
#include "DXRenderer.h"
#include "Image.h"
#include "Path.h"
#include "StringHelper.h"
#include <io.h>

SingletonCpp(ImageManager)

/********************************************************************************
## ImageManager ## 
********************************************************************************/
ImageManager::ImageManager()
{
	this->SetCoverter(_DXRenderer->GetFormatConverter());
	this->SetFactory(_DXRenderer->GetImageingFactory());
}
/********************************************************************************
## ~ImageManager ## 
모든 리소스 삭제 
********************************************************************************/

ImageManager::~ImageManager()
{
	this->DeleteAll();
}

/********************************************************************************
## AddImage ## 
이미지 등록
@@ string key : 이미지 키값
@@ wstring file : 파일경로
@@ bool usePixelCollisiton : 픽셀 충돌할것인지 

@@return Image* : 새로 추가한 이미지 반환 
********************************************************************************/
Image * ImageManager::AddImage(string key, wstring file, bool usePixelCollision)
{
	Image* image = this->FindImage(key);
	if (image)
		return image;

	ID2D1Bitmap* bitmap = this->CreateD2DBitmapFromFile(file);
	if (bitmap)
	{
		Image::TagLoadedImageInfo loadInfo = { key,file };
		image = new Image;
		image->Init(bitmap, loadInfo);
		
		if (usePixelCollision)
		{
			HBITMAP hBit = (HBITMAP)LoadImageW(Window::GetWindowDesc().Instance, file.c_str(), IMAGE_BITMAP,
				bitmap->GetPixelSize().width, bitmap->GetPixelSize().height, LR_LOADFROMFILE);
			HDC memDC = CreateCompatibleDC(Window::GetWindowDesc().Hdc);

			SelectObject(memDC, hBit);
			DeleteObject(hBit);

			image->SetPixelDC(memDC);
		}
		imageList.insert(make_pair(key, image));
		return image;
	}
	else
		assert(E_FAIL);

	return nullptr;
}

/********************************************************************************
## AddFrameImage ##
프레임 이미지 등록 
@@ string key : 키값 
@@ wstring file : 파일경로 
@@ int maxFrameX : 가로프레임 수 
@@ int maxFrameY : 세로 프레임 수
@@ bool usePixel : 픽셀 충돌용으로 쓸것인지 

@@ return Image* : 등록한 이미지 반환
********************************************************************************/

Image * ImageManager::AddFrameImage(string key, wstring file, int maxFrameX, int maxFrameY, bool usePixelCollision)
{
	Image* image = this->FindImage(key);
	if (image)
		return image;

	ID2D1Bitmap* bitmap = CreateD2DBitmapFromFile(file);
	if (bitmap)
	{
		Image::TagLoadedImageInfo loadInfo = { key,file };
		image = new Image;
		image->Init(bitmap, loadInfo, maxFrameX, maxFrameY);

		if (usePixelCollision)
		{
			HBITMAP hBit = (HBITMAP)LoadImageW(Window::GetWindowDesc().Instance, file.c_str(), IMAGE_BITMAP,
				bitmap->GetPixelSize().width, bitmap->GetPixelSize().height, LR_LOADFROMFILE);
			HDC memDC = CreateCompatibleDC(Window::GetWindowDesc().Hdc);

			SelectObject(memDC, hBit);
			DeleteObject(hBit);

			image->SetPixelDC(memDC);
		}

		this->imageList.insert(make_pair(key, image));
		return image;
	}

	return nullptr;
}

/********************************************************************************
## FineImage ## 
이미지 키값으로 찾아 반환 
@@ string key : 키값

@@ return Image* : 찾은 이미지 반환 
********************************************************************************/
Image * ImageManager::FindImage(string key)
{
	MapImageIter iter = imageList.find(key);
	if (iter != imageList.end())
		return iter->second;

	return nullptr;
}

/********************************************************************************
## GetImage ## 
이미지 키로 바로 접근해서 반환 (키로 직접적으로 접근하면 만약 없는 키값일 시 해쉬맵에 빈공간을 
생성하니 주의) 
@@ string key : 키값 

@@ return Image* : 찾은 이미지 반환
********************************************************************************/
Image * ImageManager::GetImage(string key)
{
	return this->imageList[key];
}

/********************************************************************************
## CreateD2DBitmapFromFile ##
경로로부터 팩토리와 디코더를 통해 비트맵을 생성
@@ wstring file : 파일 경로 

@@ return ID2D1Bitmap* : 생성한 비트맵 반환 
********************************************************************************/
ID2D1Bitmap * ImageManager::CreateD2DBitmapFromFile(wstring file)
{
	//디코더 생성
	IWICBitmapDecoder* ipDecoder = nullptr;
	wicFactory->CreateDecoderFromFilename(file.c_str(), NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, &ipDecoder);
	//디코더에서 프레임얻음
	IWICBitmapFrameDecode* ipFrame = nullptr;
	ipDecoder->GetFrame(0, &ipFrame);
	//프레임을 기반으로 포맷 컨버터를 만든다.
	SafeRelease(convertedSrcBmp);
	wicFactory->CreateFormatConverter(&convertedSrcBmp);

	convertedSrcBmp->Initialize(
		ipFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	//컨버트된 데이터를 기반으로 실제 Direct2D용 비트맵을 생성
	ID2D1Bitmap* ipResult = nullptr;
	_DXRenderer->GetD2DRenderTarget()->CreateBitmapFromWicBitmap
	(
		convertedSrcBmp,
		nullptr,
		&ipResult
	);

	SafeRelease(ipDecoder);
	SafeRelease(ipFrame);

	return ipResult;
}

/********************************************************************************
## DeleteAll ## 
전체 이미지 클래스 할당 해제 
********************************************************************************/
void ImageManager::DeleteAll()
{
	MapImageIter iter = imageList.begin();
	for (; iter != imageList.end(); ++iter)
	{
		if(iter->second)
			iter->second->Release();
		SafeDelete(iter->second);
	}
	imageList.clear();
}

/********************************************************************************
## OnGui ## 
********************************************************************************/
void ImageManager::OnGui()
{

}

/********************************************************************************
## LoadFolder ## 
해당 폴더의 모든 png를 찾아 이미지로 등록 
@@ const string path : 파일 경로
@@ const string filter : 찾을 파일 필터 
********************************************************************************/
void ImageManager::LoadFolder(const string path, const string filter)
{
	std::string searching = path + filter;

	std::vector<std::string> return_;

	_finddata_t fd;
	long handle = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)return;

	int result = 0;
	do
	{
		string filePath = path + fd.name;

		if (filter == "*.png")
			this->AddImage(Path::GetFileNameWithoutExtension(fd.name),StringHelper::StringToWString(filePath));

		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
}

/********************************************************************************
## LoadAllResource ##
********************************************************************************/
void ImageManager::LoadAllResource()
{
	//this->LoadFolder("../Resources/", "*.png");
	//this->LoadFolder("../Resources/UI/", "*.png");
	//this->LoadFolder("../Resources/Object/", "*.png");
	//this->LoadFolder("../Resources/Tile/Brick/", "*.png");
	//this->LoadFolder("../Resources/Tile/Dessert/", "*.png");
	//this->LoadFolder("../Resources/Tile/Dirt/", "*.png");
	//this->LoadFolder("../Resources/Tile/Etc/", "*.png");
	//this->LoadFolder("../Resources/Tile/Grass/", "*.png");
	//this->LoadFolder("../Resources/Tile/Row/", "*.png");
	//this->LoadFolder("../Resources/Tile/Snow/", "*.png");
	//this->LoadFolder("../Resources/Tile/Stone/", "*.png");
	//this->LoadFolder("../Resources/Tile/Water/", "*.png");
	//this->LoadFolder("../Resources/Tile/Wood/", "*.png");

	//this->AddFrameImage("PlayerTest", PathResource(L"FrameImage/PlayerTest.png"), 8, 2);
	//this->AddFrameImage("PlayerAttack", PathResource(L"FrameImage/PlayerAttack.png"), 10, 2);
	//this->AddFrameImage("PlayerWalk", PathResource(L"FrameImage/PlayerWalk.png"), 8, 8);
}
