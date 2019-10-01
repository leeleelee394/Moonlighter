#include "stdafx.h"
#include "Inventory.h"
#include "StringHelper.h"
#include "Image.h"
#include "Player.h"
Inventory::Inventory()
{
	this->SetActive(false);
}


Inventory::~Inventory()
{
}

void Inventory::Init()
{
	/*********************************
			  인벤토리 초기화
	**********************************/
	//이름
	this->_name = "Inventory";

	//이미지 추가
	//LSTRING /.. 은 이전 폴더로 이동 /로 타고 타고 넘어갈 수 있다. png 파일도 사용 가능
	//false는 픽셀 충돌 할 건지 말 건지 체크용
	this->_inventoryImage = _ImageManager->FindImage("InventoryWindow");

	//인벤토리 위치 잡기
	//X 포지션은 화면 가로 중앙
	//Y 포지션은 화면 맨 밑 + 이미지 높이만큼    why? 인벤토리는 처음에는 숨겨져 있기 때문.
	this->_position = Vector2(WinSizeX / 2, WinSizeY +_inventoryImage->GetHeight());

	//인벤토리 크기 잡기
	//X 크기는 인벤토리 이미지 가로 길이만큼     (int)는 형변환
	//Y 크기는 인벤토리 이미지 세로 길이만큼     (int)는 형변환
	this->_size = Vector2((int)_inventoryImage->GetWidth(), (int)_inventoryImage->GetHeight());

	//인벤토리 생성할 지점
	//중앙(CENTER)에 생성 
	this->_pivot = Pivot::CENTER;

	//중복되는 아이템 갯수 체크용
	this->_itemCount = 0;

	//업데이트 렉트 처리
	this->UpdateMainRect();
	//233 185

	//플레이어 슬롯 렉트 생성
	//플레이어 슬롯은 5칸
	for (UINT i = 0; i < 5; ++i)
	{
		//인벤토리 슬롯 클래스 포인터 slot = 인벤토리 클래스 NEW 선언
		InventorySlot* slot = new InventorySlot;
		
		//슬롯 -> 슬롯 렉트 = left top 기준으로 렉트 생성. 
		//X시작 좌표는 메인 렉트.left + 95 + (57 + 10) * i 만큼 간격으로 5번 생성, 즉 5개의 렉트 생성
		//Y시작 좌표는 메인 렉트.top + 58

		//플레이어 슬롯 사이즈 선언
		//X,Y 크기는 57, 57
		slot->size = Vector2(57, 57);

		//플레이어 슬롯 렉트 생성
		slot->slotRect = Figure::RectMake(_mainRect.left +  95 + (57 + 10) * i, _mainRect.top + 58, slot->size.x, slot->size.y);

		//플레이어 슬롯 아이템 카운트 초기화
		slot->itemCount = 0;

		//아직 아이템이 들어가지 않았으므로 슬롯 isEmpty는 true
		slot->isEmpty = true;
		
		//렉트가 5개 생성되고 플레이어 인벤토리 슬롯도 5개 생성 필요하기 때문에
		//push_back에 new 선언한 slot을 담는다.
		_playerSlotList.push_back(slot);
	}

	//가방 슬롯 렉트 생성
	//가방 슬롯은 15칸
	for (UINT i = 0; i < 3; ++i) 
	{
		for (UINT j = 0; j < 5; ++j) 
		{
			//인벤토리 슬롯 new 선언
			InventorySlot* bagSlot = new InventorySlot;

			//가방 슬롯 사이즈 선언
			bagSlot->size = Vector2(57, 57);

			//가방 슬롯 렉트 15칸 생성
			bagSlot->slotRect = Figure::RectMake(_mainRect.left + 95 + (57 + 10) * j, _mainRect.top + 144 + (58 + 10) * i, bagSlot->size.x, bagSlot->size.y);

			//가방 슬롯은 처음에 비었으므로 isEmpty는 true
			bagSlot->isEmpty = true;

			//가방 슬롯 아이템 카운트 초기화
			bagSlot->itemCount = 0;

			//생성한 슬롯을 가방 슬롯 리스트에 push_back담기
			_bagSlotList.push_back(bagSlot);
		}
	}

	//무기 슬롯 렉트 생성
	//무기 슬롯은 2칸
	for (UINT i = 0; i < 2; ++i) 
	{
		//인벤토리 슬롯 new 선언
		InventorySlot* weaponSlot = new InventorySlot;

		//무기 슬롯 사이즈 선언
		weaponSlot->size = Vector2(57, 57);

		//무기 슬롯 렉트 2칸 생성
		weaponSlot->slotRect = Figure::RectMake(_mainRect.left + 680 + (57 + 50)* i, _mainRect.top + 50, weaponSlot->size.x, weaponSlot->size.y);

		//무기 슬롯은 처음에 비었으므로 isEmpty는 true
		weaponSlot->isEmpty = true;

		//생성한 무기 슬롯을 무기 슬롯 리스트에 push_back담기
		_weponSlotList.push_back(weaponSlot);
	}

	//장비(방어구) 슬롯 렉트 생성
	//장비 슬롯은 3칸
	for (UINT i = 0; i < 3; ++i) 
	{
		//인벤토리 슬롯 new 선언
		InventorySlot* equipSlot = new InventorySlot;

		//장비 슬롯 사이즈 선언
		equipSlot->size = Vector2(57, 57);

		//장비 슬롯 렉트 세로로 3칸 생성
		equipSlot->slotRect = Figure::RectMake(_mainRect.left + 600 + 57, _mainRect.top + 144 + (58 + 10) * i, equipSlot->size.x, equipSlot->size.y);

		//장비 슬롯은 처음에 비었으므로 isEmpty는 true
		equipSlot->isEmpty = true;

		//생성한 장비 슬롯을 장비 슬롯 리스트에 push_back담기
		_equipSlotList.push_back(equipSlot);
	}

	//포션 슬롯 렉트 생성
	//포션 슬롯은 1칸
	//인벤토리 슬롯 new선언
	InventorySlot* potionSlot = new InventorySlot;

	potionSlot->size = Vector2(57, 57);

	//포션 슬롯 렉트 1칸 생성
	potionSlot->slotRect = Figure::RectMake(_mainRect.left + 667 + 57, _mainRect.top + 200 + 58, potionSlot->size.x, potionSlot->size.y);
	//포션 슬롯은 처음에 비었으므로 isEmpty는 true
	potionSlot->isEmpty = true;
	//생성한 포션 슬롯을 포션 슬롯 리스트에 push_back담기
	_potionSlotList.push_back(potionSlot);

	//인벤토리 타겟 이미지 추가
	this->_invenTargetImg = _ImageManager->AddImage("InventoryTarget", L"../Resources/UI/invenSlot.png", false);

	//인벤 타겟 포지션은 항상 시작은 플레이어 슬롯 첫번째 칸부터 시작
	_invenTargetPosition = _playerSlotList[0]->slotRect.left, _playerSlotList[0]->slotRect.top;

	//인벤토리 인덱스
	_invenIndex = 0;

	//인벤 슬롯 상태 변경위한 bool
	_isInvenSlot = false;

	/*********************************
		플레이어 현재 상태 가져오기
	**********************************/

	//윌 이미지 담기
	_invenPlayer = _ImageManager->FindImage("Will");

	//인벤토리 플레이어 포지션 
	_invenPlayerPosition = Vector2(787, WinSizeY + 500);

	//인벤토리 플레이어 렉트 생성
	_invenPlayerRect = Figure::RectMake(_invenPlayerPosition.x, _invenPlayerPosition.y, 290, 290);


	//플레이어 사망 시 메시지 받기
	this->AddCallbackMessage("PlayerDie", [this](TagMessage message)
	{
		this->SprayItemPlayer();
	});


	//인벤토리 돋보기 이미지
	this->_invenGlasses = _ImageManager->FindImage("InvenGlasses");

	//인벤토리 돋보기 포지션
	this->_invenGlassesPosition = Vector2(70, 447 + 895);

	//인벤토리 돋보기 렉트
	this->_invenGlassesRect = Figure::RectMake(_invenGlassesPosition.x, _invenGlassesPosition.y, 50, 50);

	////테스트용
	//for (UINT i = 0; i < 200; ++i)
	//	this->AddItem("item_brench");

} 

//인벤토리 Release
void Inventory::Release()
{
	//플레이어 슬롯 생성한 만큼(size) SafeDelete해주기
	for (UINT i = 0; i < _playerSlotList.size(); ++i)
	{
		SafeDelete(_playerSlotList[i]);
	}

	//가방 슬롯 생성한 만큼(size) SafeDelete 해주기
	for (UINT i = 0; i < _bagSlotList.size(); ++i) 
	{
		SafeDelete(_bagSlotList[i]);
	}
}


//인벤토리 Update
void Inventory::Update()
{	
	//인벤토리 상태 함수 사용
	InvenState();
	
	//타겟 함수 사용
	InvenTarget();

}

void Inventory::Render()
{
	//만일 인벤토리 이미지에 값이 없지 않으면 (=값이 있으면) 렌더하기
	if (_inventoryImage != nullptr)
		_inventoryImage->Render(_mainRect.left, _mainRect.top, Pivot::LEFT_TOP, false);

	//만일 인벤토리 타겟 이미지 값이 없지 않으면(=값이 있으면) 렌더하기
	if (_invenTargetImg != nullptr) 
	{
		//캐릭터 슬롯 인벤토리 상태, PlayerTarget
		if (_targetState == InvenTargetState::PlayerTarget) 
		{
			_invenTargetImg->SetSize(_playerSlotList[_invenIndex]->GetSize());
			_invenTargetImg->Render(_playerSlotList[_invenIndex]->slotRect.left, _playerSlotList[_invenIndex]->slotRect.top, Pivot::LEFT_TOP, false);
		}
		
		//가방 슬롯 인벤토리 상태, BagTarget
		if (_targetState == InvenTargetState::BagTarget)
		{
			_invenTargetImg->SetSize(_bagSlotList[_invenIndex]->GetSize());
			_invenTargetImg->Render(_bagSlotList[_invenIndex]->slotRect.left, _bagSlotList[_invenIndex]->slotRect.top, Pivot::LEFT_TOP, false);
		}

		//무기 슬롯 인벤토리 상태, WeaponTarget
		if (_targetState == InvenTargetState::WeaponTarget) 
		{
			_invenTargetImg->SetSize(_weponSlotList[_invenIndex]->GetSize());
			_invenTargetImg->Render(_weponSlotList[_invenIndex]->slotRect.left, _weponSlotList[_invenIndex]->slotRect.top, Pivot::LEFT_TOP, false);
		}

		//장비(방어구) 슬롯 인벤토리 상태, EquipTarget
		if (_targetState == InvenTargetState::EquipTarget) 
		{
			_invenTargetImg->SetSize(_equipSlotList[_invenIndex]->GetSize());
			_invenTargetImg->Render(_equipSlotList[_invenIndex]->slotRect.left, _equipSlotList[_invenIndex]->slotRect.top, Pivot::LEFT_TOP, false);
		}

		//포션 슬롯 인벤토리 상태
		if (_targetState == InvenTargetState::PotionTarget) 
		{
			_invenTargetImg->SetSize(_playerSlotList[_invenIndex]->GetSize());
			_invenTargetImg->Render(_potionSlotList[_invenIndex]->slotRect.left, _potionSlotList[_invenIndex]->slotRect.top, Pivot::LEFT_TOP, false);
		}
	}
	//렉트 토글 F1
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, false);
	}

	//플레이어 슬롯 사이즈만큼 
	for (UINT i = 0; i < _playerSlotList.size(); ++i)
	{
		//렉트 토글 F1
		if (_isDebug)
			_DXRenderer->DrawRectangle(_playerSlotList[i]->slotRect, DefaultBrush::red, false);

		//슬롯에 아이템이 들어왔다면
		if(_playerSlotList[i]->data.image)
		{
			//아이템 이미지 크기를 슬롯 렉트에 맞춰서 슬롯 렉트에 담기
			Vector2 size = Vector2(_playerSlotList[i]->slotRect.right - _playerSlotList[i]->slotRect.left,
				_playerSlotList[i]->slotRect.bottom - _playerSlotList[i]->slotRect.top);

			_playerSlotList[i]->data.image->SetSize(size);

			_playerSlotList[i]->data.image->Render(_playerSlotList[i]->slotRect.left, _playerSlotList[i]->slotRect.top, Pivot::LEFT_TOP, false);
		}

		//플레이어 슬롯 리스트 아이템 카운트가 1보다 크면
		if (_playerSlotList[i]->itemCount > 1) 
		{
			//텍스트 렌더
			_DXRenderer->RenderText(_playerSlotList[i]->slotRect.right - _playerSlotList[i]->size.x / 2.8f,
				_playerSlotList[i]->slotRect.bottom - _playerSlotList[i]->size.y / 3.0f,
				StringHelper::StringToWString(to_string(_playerSlotList[i]->itemCount)),RGB(13,13,13),1.0f,18);
		}
		
	}

	//가방 슬롯 사이즈만큼
	for (UINT i = 0; i < _bagSlotList.size(); ++i) 
	{
		//렉트 토글 F1
		if (_isDebug)
			_DXRenderer->DrawRectangle(_bagSlotList[i]->slotRect, DefaultBrush::blue, false);

		//가방 슬롯 그리기
		if (_bagSlotList[i]->data.image) 
		{
			//슬롯 오른쪽에서 왼쪽 빼기, 슬롯 밑에서 위 빼기 해서 슬롯 렉트 사이즈 구하기
			Vector2 size = Vector2(_bagSlotList[i]->slotRect.right - _bagSlotList[i]->slotRect.left,
				_bagSlotList[i]->slotRect.bottom - _bagSlotList[i]->slotRect.top);

			//구한 사이즈를 data.image의 SetSize에 담아주기
			_bagSlotList[i]->data.image->SetSize(size);

			//담은 size크기에 맞춰 data.image 렌더해주기
			_bagSlotList[i]->data.image->Render(_bagSlotList[i]->slotRect.left, _bagSlotList[i]->slotRect.top, Pivot::LEFT_TOP, false);
		}

		//가방 슬롯 리스트 아이템 카운트가 1보다 크면
		if (_bagSlotList[i]->itemCount > 1)
		{
			//텍스트 렌더
			_DXRenderer->RenderText(_bagSlotList[i]->slotRect.right - _bagSlotList[i]->size.x / 2.8f,
				_bagSlotList[i]->slotRect.bottom - _bagSlotList[i]->size.y / 3.0f,
				StringHelper::StringToWString(to_string(_bagSlotList[i]->itemCount)), RGB(13, 13, 13), 1.0f, 18);
		}
	}

	//무기 슬롯 사이즈만큼
	for (UINT i = 0; i < _weponSlotList.size(); ++i) 
	{
		//렉트 토글 F1
		if (_isDebug) 
		{
			_DXRenderer->DrawRectangle(_weponSlotList[i]->slotRect, DefaultBrush::green, false);
		}

		//무기 슬롯 그리기
		if (_weponSlotList[i]->data.image) 
		{
			//슬롯 렉트 사이즈 구하기
			Vector2 size = Vector2(_weponSlotList[i]->slotRect.right - _weponSlotList[i]->slotRect.left,
				_weponSlotList[i]->slotRect.bottom - _weponSlotList[i]->slotRect.top);

			//구한 사이즈를 data.image의 SetSize에 담아주기
			_weponSlotList[i]->data.image->SetSize(size);

			//담은 size 크기에 맞춰 data.image 렌더해주기
			_weponSlotList[i]->data.image->Render(_weponSlotList[i]->slotRect.left, _weponSlotList[i]->slotRect.top, Pivot::LEFT_TOP, false);
		}
	}

	//장비 슬롯 사이즈만큼
	for (UINT i = 0; i < _equipSlotList.size(); ++i) 
	{
		//렉트 토글 F1
		if (_isDebug) 
		{
			_DXRenderer->DrawRectangle(_equipSlotList[i]->slotRect, DefaultBrush::yello, false);
		}

		//장비 슬롯 그리기
		if (_equipSlotList[i]->data.image) 
		{
			//슬롯 렉트 사이즈 구하기
			Vector2 size = Vector2(_equipSlotList[i]->slotRect.right - _equipSlotList[i]->slotRect.left,
				_equipSlotList[i]->slotRect.bottom - _equipSlotList[i]->slotRect.top);

			//구한 사이즈를 data.image의 SetSize에 담기
			_equipSlotList[i]->data.image->SetSize(size);

			//담은 size 크기에 맞춰 data.image 렌더해주기
			_equipSlotList[i]->data.image->Render(_equipSlotList[i]->slotRect.left, _equipSlotList[i]->slotRect.top, Pivot::LEFT_TOP, false);
		}
	}

	//포션 슬롯 사이즈만큼
	for (UINT i = 0; i < _potionSlotList.size(); ++i)
	{
		//렉트 토글 F1
		if (_isDebug)
		{
			_DXRenderer->DrawRectangle(_potionSlotList[i]->slotRect, DefaultBrush::gray, false);
		}

		//장비 슬롯 그리기
		if (_potionSlotList[i]->data.image)
		{
			//슬롯 렉트 사이즈 구하기
			Vector2 size = Vector2(_potionSlotList[i]->slotRect.right - _potionSlotList[i]->slotRect.left,
				_potionSlotList[i]->slotRect.bottom - _potionSlotList[i]->slotRect.top);

			//구한 사이즈를 data.image의 SetSize에 담기
			_potionSlotList[i]->data.image->SetSize(size);

			//담은 size 크기에 맞춰 data.image 렌더해주기
			_potionSlotList[i]->data.image->Render(_potionSlotList[i]->slotRect.left, _potionSlotList[i]->slotRect.top, Pivot::LEFT_TOP, false);
		}
	}

	/*********************************
		플레이어 현재 상태 가져오기
	**********************************/
	//플레이어 포인터 플레이어에 다운캐스팅 적용해서 함수 가져오기
	Player* _player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");

	//사이즈 선언
	_invenPlayer->SetSize(Vector2(290, 290));

	//프레임 렌더
	_invenPlayer->FrameRender(_invenPlayerPosition.x, _invenPlayerPosition.y, _player->GetPlayerIndex().x, _player->GetPlayerIndex().y, Pivot::CENTER, false);

	//인벤토리 돋보기 렌더
	_invenGlasses->Render(_invenGlassesPosition.x, _invenGlassesPosition.y, Pivot::LEFT_TOP, false);

	//렉트 토글 F1
	if (_isDebug)
	{
		_DXRenderer->DrawRectangle(_invenGlassesRect, DefaultBrush::red, false);
	}
}



//습득한 아이템 정보 추가 
bool Inventory::AddItem(string name)
{
	//만일 아이템 이름이 item_brench이면
	if (name == "item_brench")
	{
		//플레이어 슬롯 사이즈만큼
		for (UINT i = 0; i < _playerSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있지 않으면
			if (_playerSlotList[i]->itemCount > 0)
			{
				//만일 이미지 키 이름과 name이 같다면	
				if (_playerSlotList[i]->data.image->GetKey() == name)
				{
					//아이템 카운트가 5개보다 낮으면
					if (_playerSlotList[i]->itemCount < 5) 
					{
						//플레이어 슬롯 아이템 카운트 증가
						_playerSlotList[i]->itemCount++;
						return true;
					}
				}
			}
		}

		//플레이어 슬롯 사이즈만큼
		for (UINT i = 0; i < _playerSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있으면
			if (_playerSlotList[i]->itemCount == 0)
			{
				//해당 아이템 이미지 찾기
				_playerSlotList[i]->data.image = _ImageManager->FindImage(name);

				//플레이어 슬롯 아이템 카운트 증가
				_playerSlotList[i]->itemCount++;
				return true;
			}
			
		}

		//가방 슬롯 사이즈만큼
		for (UINT i = 0; i < _bagSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있지 않으면
			if (_bagSlotList[i]->itemCount > 0)
			{
				//만일 이미지 키 이름과 name이 같다면	
				if (_bagSlotList[i]->data.image->GetKey() == name)
				{
					//아이템 카운트가 5개보다 낮으면
					if (_bagSlotList[i]->itemCount < 5)
					{
						//플레이어 슬롯 아이템 카운트 증가
						_bagSlotList[i]->itemCount++;
						return true;
					}
				}
			}
		}

		//가방 슬롯 사이즈만큼
		for (UINT i = 0; i < _bagSlotList.size(); ++i)
		{
			//가방 슬롯 해당 칸이 비어있으면
			if (_bagSlotList[i]->itemCount == 0)
			{
				//해당 아이템 이미지 찾기
				_bagSlotList[i]->data.image = _ImageManager->FindImage(name);

				//플레이어 슬롯 아이템 카운트 증가
				_bagSlotList[i]->itemCount++;
				return true;
			}
		}
	}

	//만일 아이템 이름이 item_golemCore이면
	if (name == "item_golemCore")
	{
		//플레이어 슬롯 사이즈만큼
		for (UINT i = 0; i < _playerSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있지 않으면
			if (_playerSlotList[i]->itemCount > 0)
			{
				//만일 이미지 키 이름과 name이 같다면	
				if (_playerSlotList[i]->data.image->GetKey() == name)
				{
					//아이템 카운트가 5개보다 낮으면
					if (_playerSlotList[i]->itemCount < 5)
					{
						//플레이어 슬롯 아이템 카운트 증가
						_playerSlotList[i]->itemCount++;
						return true;
					}
				}
			}
		}

		//플레이어 슬롯 사이즈만큼
		for (UINT i = 0; i < _playerSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있으면
			if (_playerSlotList[i]->itemCount == 0)
			{
				//해당 아이템 이미지 찾기
				_playerSlotList[i]->data.image = _ImageManager->FindImage(name);

				//플레이어 슬롯 아이템 카운트 증가
				_playerSlotList[i]->itemCount++;
				return true;
			}
		}


		//가방 슬롯 사이즈만큼
		for (UINT i = 0; i < _bagSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있지 않으면
			if (_bagSlotList[i]->itemCount > 0)
			{
				//만일 이미지 키 이름과 name이 같다면	
				if (_bagSlotList[i]->data.image->GetKey() == name)
				{
					//아이템 카운트가 5개보다 낮으면
					if (_bagSlotList[i]->itemCount < 5)
					{
						//플레이어 슬롯 아이템 카운트 증가
						_bagSlotList[i]->itemCount++;
						return true;
					}
				}
			}
		}

		//가방 슬롯 사이즈만큼
		for (UINT i = 0; i < _bagSlotList.size(); ++i)
		{
			//가방 슬롯 해당 칸이 비어있으면
			if (_bagSlotList[i]->itemCount == 0)
			{
				//해당 아이템 이미지 찾기
				_bagSlotList[i]->data.image = _ImageManager->FindImage(name);

				//플레이어 슬롯 아이템 카운트 증가
				_bagSlotList[i]->itemCount++;
				return true;
			}

		}
	}

	//만일 아이템 이름이 item_redCristal이면
	if (name == "item_redCristal")
	{
		//플레이어 슬롯 사이즈만큼
		for (UINT i = 0; i < _playerSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있지 않으면
			if (_playerSlotList[i]->itemCount > 0)
			{
				//만일 이미지 키 이름과 name이 같다면	
				if (_playerSlotList[i]->data.image->GetKey() == name)
				{
					//아이템 카운트가 5개보다 낮으면
					if (_playerSlotList[i]->itemCount < 5)
					{
						//플레이어 슬롯 아이템 카운트 증가
						_playerSlotList[i]->itemCount++;
						return true;
					}
				}
			}
		}

		//플레이어 슬롯 사이즈만큼
		for (UINT i = 0; i < _playerSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있으면
			if (_playerSlotList[i]->itemCount == 0)
			{
				//해당 아이템 이미지 찾기
				_playerSlotList[i]->data.image = _ImageManager->FindImage(name);

				//플레이어 슬롯 아이템 카운트 증가
				_playerSlotList[i]->itemCount++;
				return true;
			}

		}


		//가방 슬롯 사이즈만큼
		for (UINT i = 0; i < _bagSlotList.size(); ++i)
		{
			//플레이어 슬롯 해당 칸이 비어있지 않으면
			if (_bagSlotList[i]->itemCount > 0)
			{
				//만일 이미지 키 이름과 name이 같다면	
				if (_bagSlotList[i]->data.image->GetKey() == name)
				{
					//아이템 카운트가 5개보다 낮으면
					if (_bagSlotList[i]->itemCount < 5)
					{
						//플레이어 슬롯 아이템 카운트 증가
						_bagSlotList[i]->itemCount++;
						return true;
					}
				}
			}
		}

		//가방 슬롯 사이즈만큼
		for (UINT i = 0; i < _bagSlotList.size(); ++i)
		{
			//가방 슬롯 해당 칸이 비어있으면
			if (_bagSlotList[i]->itemCount == 0)
			{
				//해당 아이템 이미지 찾기
				_bagSlotList[i]->data.image = _ImageManager->FindImage(name);

				//플레이어 슬롯 아이템 카운트 증가
				_bagSlotList[i]->itemCount++;
				return true;
			}
		}
	}
}

//인벤토리 상태 처리를 위한 switch문
void Inventory::InvenState()
{
	//스윗치문 _state : 인벤토리 상태 처리
	switch (_state)
	{
		//인벤토리 열기
	case Inventory::InventoryState::OpenSlide:

		//인벤토리 포지션 y 위로 2500.0f 만큼 이동하기
		_position.y -= 2500.0f * _TimeManager->DeltaTime();

		//인벤토리 돋보기 포지션 y 위로 2500.0f 만큼 이동하기
		_invenGlassesPosition.y -= 2500.0f * _TimeManager->DeltaTime();

		//인벤토리 포지션 y 위로 2500.0f 만큼 이동하기
		_invenPlayerPosition.y -= 2500.0f * _TimeManager->DeltaTime();

		//### 인벤토리가 계속 올라가는 걸 방지하기 위함 ###
		//만일 포지션 y가 화면 중앙이거나 더 작으면  
		if (_position.y <= WinSizeY / 2)
		{
			//포지션 y를 화면 중앙에 고정시키고
			_position.y = WinSizeY / 2;
			_invenPlayerPosition.y = WinSizeY / 2;
			_invenGlassesPosition.y = WinSizeY / 2 + 88;

			//인벤토리 상태 유지하기
			_state = InventoryState::Idle;
		}
		this->UpdateMainRect();

		//플레이어 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 5; ++i)
		{
			_playerSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 95 + (57 + 10) * i, _mainRect.top + 58, 57, 57);
		}

		//가방 인벤토리 슬롯 위치 잡기
		for (UINT i = 0, index = 0; i < 3; ++i)
		{
			for (UINT j = 0; j < 5; ++j)
			{
				_bagSlotList[index]->slotRect = Figure::RectMake(_mainRect.left + 95 + (57 + 10) * j, _mainRect.top + 144 + (58 + 10) * i, 57, 57);
				index++;
			}
		}

		//무기 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 2; ++i)
		{
			_weponSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 596 + (73 + 95) * i, _mainRect.top + 57, 57, 57);
		}

		//장비(방어구) 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 3; ++i) 
		{
			_equipSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 460 + 57, _mainRect.top + 144 + (58 + 10) * i, 57, 57);
		}

		//포션 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 1; ++i)
		{
			_potionSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 460 + 57, _mainRect.top + 299 + 58, 57, 57);
		}
		break;

		//인벤토리 닫기
	case Inventory::InventoryState::CloseSlide:
		//인벤토리 포지션 y 아래로 800.0f 만큼 이동하기
		_position.y += 2500.0f * _TimeManager->DeltaTime();

		//인벤토리 돋보기 포지션 y 아래로 2500.0f 만큼 이동하기
		_invenGlassesPosition.y += 2500.0f * _TimeManager->DeltaTime();

		//인벤토리 플레이어 상태 포지션 y 아래로 2500.0f 만큼 이동하기
		_invenPlayerPosition.y += 2500.0f * _TimeManager->DeltaTime();


		//### 인벤토리가 계속 내려가는 걸 방지하기 위함 ###
		//만일 포지션 y가 화면 중앙이거나 더 작으면  
		//if (_position.y <= WinSizeY / 2)
		//{
		//	//포지션 y를 화면 중앙에 고정시키고
		//	_position.y = WinSizeY / 2;
		//	_invenPlayerPosition.y = WinSizeY / 2;
		//}

		//만일 포지션 y가 화면 밖으로 완전히 사라지면
		if (_position.y > WinSizeY + 1000)
		{
			//액티브 false 처리 (안 보이게)
			this->SetActive(false);
		}

		//렉트 업데이트
		this->UpdateMainRect();

		//플레이어 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 5; ++i)
		{
			_playerSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 95 + (57 + 10) * i, _mainRect.top + 58, 57, 57);
		}
		
		//가방 인벤토리 슬롯 위치 잡기
		for (UINT i = 0, index = 0; i < 3; ++i)
		{
			for (UINT j = 0; j < 5; ++j)
			{
				_bagSlotList[index]->slotRect = Figure::RectMake(_mainRect.left + 95 + (57 + 10) * j, _mainRect.top + 144 + (58 + 10) * i, 57, 57);
				index++;
			}
		}

		//무기 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 2; ++i)
		{
			_weponSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 596 + (73 + 95) * i, _mainRect.top + 57, 57, 57);
		}

		//장비(방어구) 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 3; ++i)
		{
			_equipSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 460 + 57, _mainRect.top + 144 + (58 + 10) * i, 57, 57);
		}

		//포션 인벤토리 슬롯 위치 잡기
		for (UINT i = 0; i < 1; ++i)
		{
			_potionSlotList[i]->slotRect = Figure::RectMake(_mainRect.left + 460 + 57, _mainRect.top + 299 + 58, 57, 57);
		}

		break;
		//인벤토리 상태 유지
	case Inventory::InventoryState::Idle:
		//만일 I 누르면 인벤토리 닫기
		if (_Input->GetKeyDown('I'))
		{
			_SoundManager->Play("InvenOpenSound", 1.f);
			_state = InventoryState::CloseSlide;

			//벡터 포인터로 오브젝트 리스트 포인터로 받기
			const vector<GameObject*>* _pObjectList = _ObjectManager->GetObjectListPointer(ObjectType::Object);

			for (UINT i = 0; i < _pObjectList->size(); ++i) 
			{
				//InventoryClose 메시지 보내기
				_pObjectList->at(i)->SendCallbackMessage(TagMessage("InventoryClose"));
			}
		}
		break;
	default:
		break;
	}
}

//인벤토리 타겟 변경을 위한 switch문
void Inventory::InvenTarget()
{
	switch (_targetState)
	{
	case Inventory::InvenTargetState::PlayerTarget:

		//키 이동 함수
		KeyMove();

		break;
	case Inventory::InvenTargetState::BagTarget:
		//키 이동 함수
		KeyMove();


		break;
	case Inventory::InvenTargetState::WeaponTarget:
		//키 이동 함수
		KeyMove();

		break;
	case Inventory::InvenTargetState::EquipTarget:
		//키 이동 함수
		KeyMove();

		break;

	case Inventory::InvenTargetState::PotionTarget:
		//키 이동 함수
		KeyMove();

		break;
	default:
		break;
	}
}

//인벤토리 활성화
void Inventory::Enable()
{
	//인벤토리 화면 오픈
	this->_state = InventoryState::OpenSlide;
	_SoundManager->Play("InvenOpenSound", 1.f);
	//인벤토리 벡터에 포인터를 붙인 오브젝트 리스트 포인터 선언
	const vector<GameObject*>* pObjectList = _ObjectManager->GetObjectListPointer(ObjectType::Object);
	
	//인벤토리가 활성화가 되면 메시지를 보내 다른 기능을 막는다.
	//벡터가 포인터가 되었으므로 [.size는 ->size가 됨]
	for (UINT i = 0; i < pObjectList->size(); ++i)
	{
		//SendCallbackMessage로 이름 키 값을 보낸다.
		pObjectList->at(i)->SendCallbackMessage(TagMessage("InventoryOpen"));
	}
}

void Inventory::KeyMove()
{
	//인벤토리 타겟 왼쪽으로 이동
	if (_Input->GetKeyDown('A'))
	{
		//타겟 사운드
		_SoundManager->Play("SelectTarget", 0.15f);

		--_invenIndex;

		/****************************
				플레이어 타겟
		*****************************/
		//만일 인벤 타겟이 플레이어인 경우
		if (_targetState == InvenTargetState::PlayerTarget) 
		{
			//만일 인덱스가 -1이면 좌측으로 이동시키기.  ※ 좌측으로 이동 시 칸이 없으면, 반대편 칸으로 이동
			if (_invenIndex == -1) 
			{
				//0보다 낮아지면 무기 칸으로 변경
				_targetState = InvenTargetState::WeaponTarget;
				_invenIndex = 1;
			}
		}
		/****************************
				  가방 타겟
		*****************************/
		else if (_targetState == InvenTargetState::BagTarget)
		{
			//만일 인덱스가 -1이면 좌측으로 이동시키기. ※ 좌측으로 이동 시 칸이 없으면, 반대편 칸으로 이동
			if (_invenIndex == -1)
			{
				//0보다 낮아지면 장비 칸으로 변경
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 4이면 좌측으로 이동시키기. ※ 추후 캐릭터 장비 인벤이 추가되면 그 칸으로 변경 예정
			else if (_invenIndex == 4) 
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 1;
			}
			//만일 인덱스가 9이면 좌측으로 이동시키기. ※ 추후 캐릭터 장비 인벤이 추가되면 그 칸으로 변경 예정
			else if (_invenIndex == 9) 
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 2;
			}
		}
		/****************************
				  무기 타겟
		*****************************/
		else if (_targetState == InvenTargetState::WeaponTarget) 
		{
			//만일 인덱스가 -1이면 좌측 캐릭터 칸으로 이동시키기.
			if (_invenIndex == -1) 
			{
				//0보다 낮아지면 캐릭터 칸으로 변경
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 4;
			}
		}

		/****************************
			   장비(방어구) 타겟
		*****************************/
		else if (_targetState == InvenTargetState::EquipTarget)
		{
			//만일 인덱스가 -1이면 가방 칸으로 이동시키기.
			if (_invenIndex == -1)
			{
				//0보다 낮아지면 가방 칸으로 변경
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 4;
			}

			//만일 인덱스가 0이면 가방 칸으로 이동시키기.
			else if (_invenIndex == 0)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 9;
			}

			//만일 인덱스가 1이면 가방 칸으로 이동시키기.
			else if (_invenIndex == 1)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 14;
			}
		}

		/****************************
			      포션 타겟
		*****************************/
		else if (_targetState == InvenTargetState::PotionTarget) 
		{
			//만일 인덱스가 -1이면 가방 칸으로 이동시키기.
			if (_invenIndex == -1)
			{
				//0보다 낮아지면 가방 칸으로 변경
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 14;
			}
		}
	}

	//인벤토리 타겟 오른쪽으로 이동
	if (_Input->GetKeyDown('D'))
	{
		//타겟 사운드
		_SoundManager->Play("SelectTarget", 0.15f);

		++_invenIndex;

		/****************************
				플레이어 타겟
		*****************************/
		//만일 인벤 타겟이 플레이어인 경우
		if (_targetState == InvenTargetState::PlayerTarget)
		{
			//만일 인덱스가 5 이면 무기 타겟으로 이동
			if (_invenIndex == 5)
			{
				_targetState = InvenTargetState::WeaponTarget;
				_invenIndex = 0;
			}
		}
		/****************************
				  가방 타겟
		*****************************/
		else if (_targetState == InvenTargetState::BagTarget)
		{
			//만일 인덱스가 5인 경우, 타겟 상태는 장비 타겟으로 넘어가기
			if (_invenIndex == 5)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 10인 경우, 타겟 상태는 장비 타겟으로 넘어가기
			else if (_invenIndex == 10)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 1;
			}

			//만일 인덱스가 15인 경우, 타겟 상태는 장비 타겟으로 넘어가기
			else if (_invenIndex == 15)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 2;
			}
		}
		/****************************
				  무기 타겟
		*****************************/
		else if (_targetState == InvenTargetState::WeaponTarget)
		{
			//만일 인덱스가 2인 경우, 캐릭터 칸으로 이동시키기.
			if (_invenIndex == 2) 
			{
				//1보다 커지면 캐릭터 칸으로 변경
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 0;
			}
		}

		/****************************
			   장비(방어구) 타겟
		*****************************/
		else if (_targetState == InvenTargetState::EquipTarget)
		{
			//만일 인덱스가 1인 경우 가방 칸으로 이동하기 
			if (_invenIndex == 1)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 2인 경우 가방 칸으로 이동하기 
			else if (_invenIndex == 2)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 5;
			}

			//만일 인덱스가 2인 경우 가방 칸으로 이동하기 
			else if (_invenIndex == 3)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 10;
			}
		}

		/****************************
			      포션 타겟
		*****************************/
		else if (_targetState == InvenTargetState::PotionTarget)
		{
			//만일 인덱스가 1인 경우, 가방 칸으로 이동시키기.
			if (_invenIndex == 1)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 10;
			}
		}
	}

	//인벤토리 타겟 위로 이동
	if (_Input->GetKeyDown('W'))
	{
		//타겟 사운드
		_SoundManager->Play("SelectTarget", 0.15f);

		_invenIndex -= 5;

		/****************************
				플레이어 타겟
		*****************************/
		//만일 인벤 타겟이 플레이어인 경우
		if (_targetState == InvenTargetState::PlayerTarget)
		{
			//만일 인덱스가 -5이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -5)	//-5
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 10;
			}

			//만일 인덱스가 -4이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -4)	//
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 11;
			}

			//만일 인덱스가 -3이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -3)	//-3
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 12;
			}

			//만일 인덱스가 -2이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -2)		//-2
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 13;
			}

			//만일 인덱스가 -1이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -1)		//-1
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 14;
			}
		}
		/****************************
				  가방 타겟
		*****************************/
		else if (_targetState == InvenTargetState::BagTarget)
		{
			//만일 인덱스가 -5이면 위로 이동시키기
			if (_invenIndex == -5)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 -4이면 위로 이동시키기
			if (_invenIndex == -4)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 1;
			}

			//만일 인덱스가 -3이면 위로 이동시키기
			if (_invenIndex == -3)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 2;
			}

			//만일 인덱스가 -2이면 위로 이동시키기
			if (_invenIndex == -2)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 3;
			}

			//만일 인덱스가 -1이면 위로 이동시키기
			if (_invenIndex == -1)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 4;
			}
		}
		/****************************
				  무기 타겟
		*****************************/
		else if (_targetState == InvenTargetState::WeaponTarget)
		{
			//만일 인덱스가 -5이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -5)
			{
				_targetState = InvenTargetState::PotionTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 -4이면 위로 이동시키기. ※ 최대 높이에서 위로 누르면 반대편 아래 칸으로 이동
			if (_invenIndex == -4)
			{
				_targetState = InvenTargetState::PotionTarget;
				_invenIndex = 0;
			}
		}

		/****************************
			   장비(방어구) 타겟
		*****************************/
		else if (_targetState == InvenTargetState::EquipTarget)
		{
			//만일 인덱스가 -5이면 위로 이동시키기
			if (_invenIndex == -5)
			{
				_targetState = InvenTargetState::WeaponTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 -4이면 위로 이동시키기
			if (_invenIndex == -4)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 -3이면 위로 이동시키기
			if (_invenIndex == -3)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 1;
			}
		}

		/****************************
				  포션 타겟
		*****************************/
		else if (_targetState == InvenTargetState::PotionTarget)
		{
			//만일 인덱스가 -5이면 위로 이동시키기
			if (_invenIndex == -5)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 2;
			}
		}
	}

	//인벤토리 타겟 아래로 이동
	if (_Input->GetKeyDown('S'))
	{
		//타겟 사운드
		_SoundManager->Play("SelectTarget", 0.15f);

		_invenIndex += 5;

		/****************************
				플레이어 타겟
		*****************************/
		//만일 인벤 타겟이 플레이어인 경우
		if (_targetState == InvenTargetState::PlayerTarget)
		{
			//만일 인덱스가 5이면 아래로 이동시키기.
			if (_invenIndex == 5)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 6이면 아래로 이동시키기.
			if (_invenIndex == 6)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 1;
			}

			//만일 인덱스가 7이면 아래로 이동시키기.
			if (_invenIndex == 7)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 2;
			}

			//만일 인덱스가 8이면 아래로 이동시키기.
			if (_invenIndex == 8)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 3;
			}

			//만일 인덱스가 9이면 아래로 이동시키기.
			if (_invenIndex == 9)
			{
				_targetState = InvenTargetState::BagTarget;
				_invenIndex = 4;
			}
		}
		/****************************
				  가방 타겟
		*****************************/
		else if (_targetState == InvenTargetState::BagTarget)
		{
			//만일 인덱스가 15이면 아래로 이동시키기. ※ 최저 저점에서 아래 누르면 반대편 위 칸으로 이동
			if (_invenIndex == 15)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 16이면 아래로 이동시키기. ※ 최저 저점에서 아래 누르면 반대편 위 칸으로 이동
			if (_invenIndex == 16)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 1;
			}

			//만일 인덱스가 17이면 아래로 이동시키기. ※ 최저 저점에서 아래 누르면 반대편 위 칸으로 이동
			if (_invenIndex == 17)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 2;
			}

			//만일 인덱스가 18이면 아래로 이동시키기. ※ 최저 저점에서 아래 누르면 반대편 위 칸으로 이동
			if (_invenIndex == 18)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 3;
			}

			//만일 인덱스가 19이면 아래로 이동시키기. ※ 최저 저점에서 아래 누르면 반대편 위 칸으로 이동
			if (_invenIndex == 19)
			{
				_targetState = InvenTargetState::PlayerTarget;
				_invenIndex = 4;
			}
		}
		/****************************
				  무기 타겟
		*****************************/
		else if (_targetState == InvenTargetState::WeaponTarget)
		{
			//만일 인덱스가 5이면 아래로 이동시키기.
			if (_invenIndex == 5)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 0;
			}

			//만일 인덱스가 6이면 아래로 이동시키기.
			if (_invenIndex == 6)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 0;
			}
		}

		/****************************
			   장비(방어구) 타겟
		*****************************/
		else if (_targetState == InvenTargetState::EquipTarget)
		{
			//만일 인덱스가 5이면 아래로 이동시키기.
			if (_invenIndex == 5)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 1;
			}

			//만일 인덱스가 6이면 아래로 이동시키기.
			if (_invenIndex == 6)
			{
				_targetState = InvenTargetState::EquipTarget;
				_invenIndex = 2;
			}

			//만일 인덱스가 7이면 아래로 이동시키기.
			if (_invenIndex == 7)
			{
				_targetState = InvenTargetState::PotionTarget;
				_invenIndex = 0;
			}
		}

		/****************************
				  포션 타겟
		*****************************/
		else if (_targetState == InvenTargetState::PotionTarget)
		{
			//만일 인덱스가 5이면 아래로 이동시키기.  ※ 최저 저점에서 아래 누르면 반대편 위 칸으로 이동
			if (_invenIndex == 5)
			{
				_targetState = InvenTargetState::WeaponTarget;
				_invenIndex = 0;
			}
		}
	}
}