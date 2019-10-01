#include "stdafx.h"
#include "Inventory.h"
#include "Player.h"
#include "StringHelper.h"
#include "Image.h"
#include "MoveItem.h"
#include "Plankton.h"

//플레이어 사망 시 아이템 뿌리기 함수
void Inventory::SprayItemPlayer()
{
	//플레이어 포인터 플레이어에 다운캐스팅 적용해서 클래스 가져오기
	Player* player = (Player*)_ObjectManager->FindObject(ObjectType::Object, "Will");

	//가방 슬롯 사이즈만큼
	for (UINT i = 0; i < _bagSlotList.size(); ++i)
	{
		//가방 슬롯이 비어있지 않으면
		if (_bagSlotList[i]->itemCount > 0)
		{
			//아이템 카운트 체크
			for (UINT i = 0; i < _bagSlotList[i]->itemCount; ++i)
			{
				//랜덤 포지션 좌표용 지역 변수 선언
				Vector2 randomPos;
				//랜덤 좌표 X -100 ~ +100
				randomPos.x = Math::Random(player->GetPosition().x - 100.f, player->GetPosition().x + 100.f);
				//랜덤 좌표 Y -100 ~ +100
				randomPos.y = Math::Random(player->GetPosition().y - 100.f, player->GetPosition().y + 100.f);
				//무브 아이템 클래스 NEW 선언 (가방 슬롯 리스트의 이미지 키 값, 랜덤 포지션 좌표)
				MoveItem* item = new MoveItem(_bagSlotList[i]->data.image->GetKey(), randomPos);
				//오브젝트 추가 (타입 : 오브젝트, 게임오브젝트 : 무브 아이템 클래스)
				_ObjectManager->AddObject(ObjectType::Object, item);
			}
			//아이템 개수 0으로 만들기
			_bagSlotList[i]->itemCount = 0;
		}
	}
}
