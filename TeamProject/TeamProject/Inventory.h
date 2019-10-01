#pragma once
#include "GameObject.h"

//구조체 -> 인벤토리 아이템 데이터 선언  ## 아이템 
struct InvenItemData
{
	class Image* image;		//아이템 이미지
	int itemIndex;			//아이템 인덱스
	string itemDescription;	//아이템 설명
	int itemAtk;			//아이템 공격력
	int itemDef;			//아이템 방어력
	int itemSpd;			//아이템 이동속도
	int itemHp;				//아이템 체력
	InvenItemData()			//생성자에서 맴버 이니셜라이즈로 이미지 nullptr 초기화 하기
		:image(nullptr) {}
};

//인벤토리 슬롯 클래스 추가로 만들기
//인벤토리에 보이는 슬롯 처리를 따로 구분하기 위함
class InventorySlot
{
private:
	friend class Inventory;	//프렌드 클래스 인벤토리 선언 --> 서로 접근 가능하게끔 프렌드 클래스 선언
	InvenItemData data;		//인벤토리 아이템 데이터 구조체 변수 선언 data
	bool isEmpty;			//아이템 슬롯이 비어있는 확인하기 위한 bool 변수 isEmpty
	RECT slotRect;			//슬롯 그림에 렉트를 생성하기 위한 렉트 변수 slotRect
	Vector2 size;			//슬롯 사이즈
	int itemCount;
public:
	//사이즈 값 얻기 함수
	Vector2 GetSize() { return size; }
};

//인벤토리 클래스 ---> GameObject를 상속받음
class Inventory :public GameObject
{
	//이넘 클래스 ItemType 선언
	//아이템 종류에 따라 처리하기 위함
	enum class ItemType 
	{
		Normal = 0,
		Equip,
		End
	};
	//이넘 클래스 InventoryState 선언
	//인벤토리 상태에 따라 처리하기 위함
	enum class InventoryState
	{
		OpenSlide = 1,								//인벤 열기
		CloseSlide = 2,								//인벤 닫기
		Idle									//인벤 유지
	};
	
	enum class InvenTargetState 
	{
		PlayerTarget = 0,						//플레이어 타겟 상태
		BagTarget,								//가방 타겟 상태
		WeaponTarget,							//무기 타겟 상태
		EquipTarget,							//장비(방어구) 타겟 상태
		PotionTarget							//포션 타겟 상태
	};


private:
	class Image* _invenTargetImg;				//인벤토리 선택 이미지
	Vector2 _invenTargetPosition;				//인벤 타겟 포지션
	int _invenIndex;							//인벤 인덱스

	int _itemCount;								//아이템 카운트 (아이템 갯수 제한용)


	bool _isInvenSlot;							//인벤 슬롯 구분위한 bool

	class Image* _inventoryImage;				//인벤토리 이미지 변수 선언
	vector<InventorySlot*> _playerSlotList;		//벡터 >> 플레이어 슬롯 리스트 변수 선언
	vector<InventorySlot*> _bagSlotList;		//벡터 >> 가방 슬롯 리스트 변수 선언
	vector<InventorySlot*> _weponSlotList;		//벡터 >> 무기 슬롯 리스트 변수 선언		
	vector<InventorySlot*> _equipSlotList;		//벡터 >> 장비 슬롯 리스트 변수 선언
	vector<InventorySlot*> _potionSlotList;		//벡터 >> 포션 슬롯 리스트 변수 선언

	InventoryState _state;						//인벤토리 상태 변수 선언
	InvenTargetState _targetState;				//인벤토리 타겟 상태 변수 선언


	//인벤토리에 플레이어 상태 표시 이미지
	class Image* _invenPlayer;					//인벤토리에 플레이어 상태 표시 이미지

	class Player* _player;						//플레이어 클래스 선언

	class Image* _invenGlasses;					//인벤토리 돋보기 이미지
	RECT _invenGlassesRect;						//인벤토리 돋보기 렉트
	Vector2 _invenGlassesPosition;				//인벤토리 돋보기 포지션

	//인벤토리에 플레이어 상태 표시용 렉트
	RECT _invenPlayerRect;						//인벤토리 플레이어 상태 표시용 렉트
	Vector2 _invenPlayerPosition;				//인벤토리 플레이어 상태 표시용 포지션
public:
	Inventory();
	~Inventory();

	void Init();
	void Release();
	void Update();
	void Render();

	bool AddItem(string name);					//반환형 bool 값, 인벤토리에 아이템 추가하기 위한 함수, 인자로 아이템 이름을 받음

	void InvenState();							//인벤토리 상태 함수
	void InvenTarget();							//타겟 상태 변경을 위한 함수

	void Enable()override;						//인벤토리 켜기 함수

	void KeyMove();								//방향 키 함수

	void ChangeStateTarget()
	{
		_targetState = InvenTargetState::PlayerTarget;
	}

	void SprayItemPlayer();						//아이템 뿌리기 함수

};

