#include "stdafx.h"
#include "TestInventory.h"

#include "TestInventorySlot.h"
#include "TestItem.h"


TestInventory::TestInventory()
	:GameObject("TestInventory",Vector2(WinSizeX / 2,0),Vector2(),Pivot::CENTER)
{
	_state = State::OpenSlide;
	_targetIndex = 0;
}


TestInventory::~TestInventory()
{
}
/***********************************************************
## Init ##
 ***********************************************************/
void TestInventory::Init()
{
	_ImageManager->AddImage("item_potion", PathResources(L"UI/item_potion.png"));
	_ImageManager->AddImage("item_liquidItem", PathResources(L"UI/item_liquidItem.png"));

	this->_windowImage = _ImageManager->AddImage("InventoryWindow", L"../Resources/UI/inventory.png", false);
	this->_targetImage = _ImageManager->AddImage("InventoryTarget", L"../Resources/UI/invenSlot.png", false);

	this->_position = Vector2(WinSizeX / 2, WinSizeY + _windowImage->GetHeight() / 2);
	this->_size = Vector2((int)_windowImage->GetWidth(), (int)_windowImage->GetHeight());
	this->UpdateMainRect();

	for (UINT i = 0; i < 5; ++i)
	{
		TestInventorySlot* slot = new TestInventorySlot(Vector2(95 + (57 + 10) * i, 58), Vector2(57, 57));
		slot->UpdatePosition(Vector2(_mainRect.left, _mainRect.top));
		_slotList.push_back(slot);
	}

	for (UINT i = 0; i < 3; ++i)
	{
		for (UINT j = 0; j < 5; ++j)
		{
			TestInventorySlot* slot = new TestInventorySlot(Vector2((int)(95 + (57 + 10) * j),(int)(144 + (58 + 10) * i)), Vector2(57, 57));
			slot->UpdatePosition(Vector2(_mainRect.left, _mainRect.top));
			_slotList.push_back(slot);
		}
	}	


	this->MoveIndex(0);
}
/***********************************************************
## Release ##
 ***********************************************************/
void TestInventory::Release()
{
	for (UINT i = 0; i < _slotList.size(); ++i)
		SafeDelete(_slotList[i]);
	_slotList.clear();
}
/***********************************************************
## Update ##
 ***********************************************************/
void TestInventory::Update()
{
	switch (_state)
	{
	case TestInventory::State::OpenSlide:
		_position.y -= 800.0f * _TimeManager->DeltaTime();
		if (_position.y <= CastingFloat(WinSizeY / 2))
		{
			_position.y = CastingFloat(WinSizeY / 2);
			_state = State::Idle;
		}
		this->UpdateMainRect();
		this->UpdateSlotPosition();
		break;
	case TestInventory::State::CloseSlide:
		_position.y += 800.0f * _TimeManager->DeltaTime();
		this->UpdateMainRect();
		this->UpdateSlotPosition();
		if (_position.y >= CastingFloat(WinSizeY + _windowImage->GetHeight() / 2))
		{
			_position.y = CastingFloat(WinSizeY + _windowImage->GetHeight() / 2);
			_state = State::OpenSlide;
			_isActive = false;
			this->UpdateMainRect();
			this->UpdateSlotPosition();

		}
		break;
	case TestInventory::State::Idle:
		if (_Input->GetKeyDown(VK_RETURN))
			_state = State::CloseSlide;
		if (_Input->GetKeyDown('D'))
			this->MoveIndex(_targetIndex + 1);
		if (_Input->GetKeyDown('A'))
			this->MoveIndex(_targetIndex -1);
		if (_Input->GetKeyDown('W'))
			this->MoveIndex(_targetIndex - 5);
		if (_Input->GetKeyDown('S'))
			this->MoveIndex(_targetIndex + 5);
		
		break;
	default:
		break;
	};

	//테스트 용 아이템 추가
	if (_Input->GetKeyDown('L'))
		this->AddItem("item_potion");
	if (_Input->GetKeyDown('K'))
		this->AddItem("item_liquidItem");
}
/***********************************************************
## Render ##
 ***********************************************************/
void TestInventory::Render()
{
	_windowImage->Render(CastingInt(_mainRect.left), CastingInt(_mainRect.top), Pivot::LEFT_TOP, false);
	for (UINT i = 0; i < _slotList.size(); ++i)
		_slotList[i]->Render();

	if (_slotList[_targetIndex])
	{
		_targetImage->SetSize(_slotList[_targetIndex]->GetSize());
		_targetImage->Render(_slotList[_targetIndex]->GetRect().left, _slotList[_targetIndex]->GetRect().top, Pivot::LEFT_TOP, false);
	}

	if (_isDebug)
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, false);
}
/***********************************************************
## UpdateSlotPosition ##
인벤토리 창이 이동했을 때 이 함수를 호출하면 슬롯들도 전부 이동시켜준다.
 ***********************************************************/
void TestInventory::UpdateSlotPosition()
{
	for (UINT i = 0; i < _slotList.size(); ++i)
		_slotList[i]->UpdatePosition(Vector2(_mainRect.left, _mainRect.top));
}

/********************************************************************************
## AddItem ##
추가할 아이템 이름을 받아서 내부적으로 인벤토리에 아이템을 추가 한다. 
예를 들어 MoveItem클래스는 움직이다가 플레이어와 충돌하게 되면 인벤토리에 자신의 이름을
넣으면서 아이템 추가를 요청한뒤 인벤토리가 공간이 있어서 아이템을 넣게 되면 자신을 삭제하면
된다.

@@ string name : 아이템 이름
********************************************************************************/
bool TestInventory::AddItem(string name)
{
	//추가할 아이템의 이름이 itemPotion이라면
	if (name == "item_potion")
	{
		//해당 아이템을 초기화
		TestItem item;
		item.SetData(name, L"이상한 포션 . . .독이 있을지도", 10, _ImageManager->FindImage(name));
		//아이템이 들어갈 수 있는 자리가 있는지 검색해본다. 
		if (TestInventorySlot* slot = FindSlot(name))
		{
			//들어갈 수 있는 자리가 있다면 해당 슬롯에 아이템을 추가한다.
			slot->AddItem(item);
			//아이템을 성공적으로 먹었다고 알려준다.
			return true;
		}
	}
	else if (name == "item_liquidItem")
	{
		TestItem item;
		item.SetData(name, L"알수없이 영롱한 빛을 내는 보석", 10, _ImageManager->FindImage(name));
		if (TestInventorySlot* slot = FindSlot(name))
		{
			slot->AddItem(item);
			return true;
		}
	}

	return false;
}
/***********************************************************
## MoveIndex ##
@@ int index : 인벤토리 타겟 마크 이동할 인덱스 
 ***********************************************************/
void TestInventory::MoveIndex(int index)
{
	if (index < 0)
		index = _slotList.size() - 1;
	else if (index >= _slotList.size())
		index = 0;

	this->_targetIndex = index;
}

/***********************************************************
## FindSlot ##
@@ string name : 아이템 이름 

@@ return TestInventorySlot* : 찾았다면 찾은 슬롯 반환 아니라면 null 반환
 ***********************************************************/
TestInventorySlot * TestInventory::FindSlot(string name)
{
	for (UINT i = 0; i < _slotList.size(); ++i)
	{
		if (_slotList[i]->GetItemName() == name)
			return _slotList[i];
	}

	for (UINT i = 0; i < _slotList.size(); ++i)
	{
		if (_slotList[i]->GetIsEmpty() == true)
			return _slotList[i];
	}

	return nullptr;
}
