#pragma once
class ImageManager
{
	SingletonHeader(ImageManager)
private:
	typedef map<string, class Image*> MapImageList;
	typedef map<string, class Image*>::iterator MapImageIter;
private:
	MapImageList imageList;
	//WIC (이미지 처리 컴포넌트)
	IWICImagingFactory*					wicFactory;
	IWICFormatConverter*				convertedSrcBmp;
public:
	class Image* AddImage(string key, wstring file, bool usePixelCollision = false);
	class Image* AddFrameImage(string key, wstring file, int maxFrameX, int maxFrameY, bool usePixelCollision = false);
	class Image* FindImage(string key);
	class Image* GetImage(string key);

	void SetFactory(IWICImagingFactory* pFactory) { this->wicFactory = pFactory; }
	void SetCoverter(IWICFormatConverter* pConverter) { this->convertedSrcBmp = pConverter; }
	void OnGui();

	void LoadFolder(const string path, const string filter);
	void LoadAllResource();
private:
	ID2D1Bitmap* CreateD2DBitmapFromFile(wstring file);
	void DeleteAll();
};

#define _ImageManager ImageManager::Get()
