#include "stdafx.h"
#include "TestInventorySlot.h"

#include "StringHelper.h"
TestInventorySlot::TestInventorySlot(Vector2 pos, Vector2 size)
	:_offsetPosition(pos), _size(size), _itemCount(0)
{
}

TestInventorySlot::~TestInventorySlot()
{
}
/***********************************************************
## AddItem ##
@@ TestItem item : 추가할 아이템 
 ***********************************************************/
void TestInventorySlot::AddItem(TestItem item)
{
	//만약 슬롯이 비어있는 슬롯이 었다면 아이템 정보를 해당 아이템 정보로 초기화 
	if (this->GetIsEmpty() == true)
		_item = item;
	//아이템 갯수 추가 
	_itemCount++;
}
/***********************************************************
## UpdatePosition ##
@@ Vector2 parentPos : 인벤토리의 좌표 
 ***********************************************************/
void TestInventorySlot::UpdatePosition(Vector2 parentPos)
{
	//내 좌표는 부모좌표 + 상대좌표 거리 
	_position = parentPos + _offsetPosition;
	_rect = Figure::RectMake(_position, _size);
}
/***********************************************************
## Render ##
 ***********************************************************/
void TestInventorySlot::Render()
{
	//아이템이 있다면 
	if (_itemCount > 0)
	{
		//이미지 그려라
		_item.GetImage()->SetSize(_size);
		_item.GetImage()->Render(CastingInt(_rect.left), CastingInt(_rect.top), Pivot::LEFT_TOP, false);
		//아이템 수가 2개 이상있다면 갯수 출력
		if (_itemCount > 1)
			_DXRenderer->RenderText(CastingInt(_rect.right - (LONG)(_size.x / 2.5f)), _rect.bottom - (LONG)(_size.y / 2.5f),
				StringHelper::StringToWString(to_string(_itemCount)), 30, DefaultBrush::white);
	}

	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_rect, DefaultBrush::red, false);
	}
}

bool TestInventorySlot::GetIsEmpty()
{
	if (_itemCount <= 0)
		return true;

	return false;
}
