#include "stdafx.h"
#include "TestItem.h"


TestItem::TestItem()
	:_name(""),_discription(L""),_defaultPrice(0), _image(nullptr)
{
}


TestItem::~TestItem()
{
}

void TestItem::SetData(string name, wstring discription, int defaultPrice, Image * image)
{
	_name = name;
	_discription = discription;
	_defaultPrice = defaultPrice;
	_image = image;
}
