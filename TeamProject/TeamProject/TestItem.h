#pragma once
class TestItem
{
private:
	class Image* _image;
	string _name; 
	wstring _discription;
	int _defaultPrice; 
public:
	TestItem();
	~TestItem();

	void SetData(string name, wstring discription, int defaultPrice,class Image* image);

	class Image* GetImage()const { return this->_image; }
	string GetName()const { return this->_name; }
	wstring GetDiscription()const { return this->_discription; }
	int GetDefaultPrice()const { return this->_defaultPrice; }
};

