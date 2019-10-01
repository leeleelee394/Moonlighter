#pragma once
#include "TestItem.h"

class TestInventorySlot
{
private:
	Vector2 _position;
	Vector2 _size; 
	RECT _rect;

	Vector2 _offsetPosition;

	TestItem _item;
	int _itemCount;
public:
	TestInventorySlot(Vector2 offsetPos,Vector2 size);
	~TestInventorySlot();

	void AddItem(TestItem item);
	void UpdatePosition(Vector2 parentPos);
	void Render();

	bool GetIsEmpty();
	TestItem GetItem()const { return this->_item; }
	string GetItemName()const { return this->_item.GetName(); }
	RECT GetRect()const { return this->_rect; }
	Vector2 GetSize()const { return this->_size; }
};

