#include "stdafx.h"
#include "MoveItem.h"
#include "UIWeaponAndBag.h"
#include "Inventory.h"
#include "Player.h"
#include "Plankton.h"
MoveItem::MoveItem(string name, Vector2 position)
{
	_name = name;
	_position = position;

	_itemImg = _ImageManager->FindImage(name);
	_itemShadowImg = _ImageManager->FindImage("Shadow");


	_pivot = Pivot::LEFT_TOP;
	_size = Vector2(50, 50);
	_mainRect = Figure::RectMake(_position.x, _position.y, _size.x, _size.y);

	_angle = 0.f;		//아이템 흔들림을 위한 앵글 변수
	_speed = 1.5f;		//아이템 흔들림 속도를 위한 속도 변수

	_itemState = ItemState::Follow;

	_moveCount = 0;
}


MoveItem::~MoveItem()
{

}

void MoveItem::Init()
{
}

void MoveItem::Release()
{
}

void MoveItem::Update()
{
	Player* _player = (Player*)_ObjectManager->FindObject(ObjectType::Object,"Will");

	//상태가 따라가기 상태이면
	if (_itemState == ItemState::Follow)
	{
		//만일 _player가 있으면
		if (_player)
		{

			if (_player->GetState() != Player::State::Die)
			{


				//거리 구하기
				float distance = Math::GetDistance(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);

				//상대 각도 구하기
				float angle = Math::GetAngle(_position.x, _position.y, _player->GetPosition().x, _player->GetPosition().y);


				//만일 거리가 250보다 멀어지면
				//아이템 둥둥 뜨게하기
				if (250 < distance)
				{
					_angle += 0.05f;
					if (_angle >= Math::PI * 2.0f)
						_angle -= Math::PI * 2.0f;

					_position.y += sinf(_angle) * 20.0f * _TimeManager->DeltaTime();
					this->UpdateMainRect();
				}

				//만일 거리가 250보다 낮고 150보다 높으면
				else if (250 > distance && distance > 150)
				{
					float speed = 20.0f;
					this->_position.x += cosf(angle) * speed * _TimeManager->DeltaTime();
					this->_position.y -= sinf(angle) * speed * _TimeManager->DeltaTime();
					this->UpdateMainRect();
				}
				//만일 거리가 150이랑 같거나 낮아지면
				else if (distance <= 150)
				{
					float speed = 150.0f;
					this->_position.x += cosf(angle) * speed * _TimeManager->DeltaTime();
					this->_position.y -= sinf(angle) * speed * _TimeManager->DeltaTime();
					this->UpdateMainRect();
				}
				//만일 거리가 10보다 낮아지면
				if (distance <= 10)
				{
					//아이템 이동 상태 변경
					_itemState = ItemState::MoveItem;
				}
			}
		}
	}

	//아이템 상태가 아이템 이동 상태이면
	else if (_itemState == ItemState::MoveItem) 
	{
		GameObject* _moveItem = _ObjectManager->FindObject(ObjectType::UI, "UIWeaponAndBag");
		//만일 아이템(_moveItem)이 있으면 
		if (_moveItem)
		{
			Vector2 moveItemPos(_moveItem->GetMainRect().left, _moveItem->GetMainRect().top);
			moveItemPos = _Camera->GetWorldVector2(moveItemPos);
			//거리 구하기
			float distanceItemBag = Math::GetDistance(_position.x, _position.y, moveItemPos.x, moveItemPos.y);

			//각도 구하기
			float angleItemBag = Math::GetAngle(_position.x, _position.y, moveItemPos.x, moveItemPos.y);

			//아이템 이동 속도
			float moveItemSpd = 950.0f;

			//구한 각도와 아이템 속도로 포지션을 이동시키기
			this->_position.x += cosf(angleItemBag) * moveItemSpd * _TimeManager->DeltaTime();
			this->_position.y -= sinf(angleItemBag) * moveItemSpd * _TimeManager->DeltaTime();
			this->UpdateMainRect();

			if (distanceItemBag <= 20)
			{
				//아이템 상태는 인벤 상태로 변환
				_itemState = ItemState::Inven;
			}
		}
	}

	//아이템 상태가 인벤이면 
	else if (_itemState == ItemState::Inven)
	{
		//Inventory* inven(인벤토리 포인터) dynamic_cast(다이나믹 캐스트) 다운 캐스팅으로 형변환해서 인벤토리에 접근한 뒤 
		//만일 인벤이 nullptr이 아니면 인벤에 addItem으로 이름을 받아 true이면  아이템을 파괴 --> 즉, 아이템 안 보이게 하기
		//난 캐릭터 충돌하고 아이템 인벤 ui에 이동되면 파괴하기
		Inventory* inven = dynamic_cast<Inventory*>(_ObjectManager->FindObject(ObjectType::UI, "Inventory"));
		if (inven != nullptr)
		{
			if (inven->AddItem(this->_name) == true)
			{
				this->Destroy();
			}
		}
	}

}

void MoveItem::Render()
{

	if (_itemState == ItemState::Follow) 
	{
		//그림자
		_itemShadowImg->SetSize(Vector2(20, 13));
		_itemShadowImg->SetAlpha(0.3f);				//알파 값
		_itemShadowImg->Render(_position.x + 3, _position.y + 34, Pivot::LEFT_TOP, true);
	}
	
	//아이템
	_itemImg->Render(_position.x, _position.y, Pivot::LEFT_TOP, true);

	if (_isDebug)
		_DXRenderer->DrawRectangle(_mainRect, DefaultBrush::red, true);



	
	//렉트 렌더 토글키
	//if (_Input->IsToggleKey(VK_F10)) 
	//{
	//	_DXRenderer->FillRectangle(this->_mainRect, DefaultBrush::yello, false);
	//}

	if (_isDebug)
	{
		//ImGui::Begin("Chan");
		//ImGui::Text("X : %d , Y : %d", (int)_Input->GetMousePosition().x,(int) _Input->GetMousePosition().y);
		//ImGui::End();
	}
	
}


