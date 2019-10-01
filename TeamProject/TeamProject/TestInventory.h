#pragma once
#include "GameObject.h"
class TestInventory : public GameObject
{
	enum class State
	{
		OpenSlide,CloseSlide,Idle
	};
private:
	vector<class TestInventorySlot*> _slotList;	//슬롯들
	class Image* _windowImage;					//인벤토리 창 이미지
	class Image* _targetImage;					//타겟 슬롯 가리키는 이미지
	int _targetIndex;							//타겟 슬롯 인덱스
	State _state;								//인벤토리 상태 
public:
	TestInventory();
	~TestInventory();

	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render()override; 

	bool AddItem(string name);

private:
	void MoveIndex(int i);
	void UpdateSlotPosition();
	TestInventorySlot* FindSlot(string name);
};

