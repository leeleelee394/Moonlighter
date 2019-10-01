#include "stdafx.h"
#include "MainScene.h"
#include "MoveItem.h"
#include "Inventory.h"
#include "UIWeaponAndBag.h"
#include "TownScene.h"
#include "Dungeon_Map_01.h"
#include "Dungeon_Map_Boss.h"
#include "TestDeadman.h"
#include "ProgressBar.h"
#include "Program.h"
#include "UIMoneyPoket.h"
#include "Effect.h"
#include "Dungeon_Map_02.h"
#include "Plankton.h"
#include "DungeonLobby.h"
#include "BossProgressBar.h"
#include "BossEffect.h"
#include "UICoin.h"
#include "UIPotion.h"
//여기서 필요한 리소스 로드 및 클래스 할당하시면 됩니다. 
//Init은 ㄴㄴ new 하고 ObjectManager에 추가 ㅇㅇ


void Program::LoadResourceChan()
{
	/**********************
		     사운드
	***********************/
	_SoundManager->AddSound("SelectTarget", "../Sounds/gui_selector_movement.wav", false, false);
	_SoundManager->AddSound("InvenOpenSound", "../Sounds/gui_inventory_mirror_action.wav", false, false);


	/**********************
			   UI
	***********************/
	//메인 인벤토리 이미지 
	_ImageManager->AddImage("InventoryWindow", L"../Resources/UI/newInventory.png", false);
	_ImageManager->AddImage("InvenGlasses", L"../Resources/UI/Bag_Mirror.png", false);

	//UI 코인
	_ImageManager->AddImage("Coin", L"../Resources/UI/coin.png", false);

	//UI 무기 스왑
	_ImageManager->AddFrameImage("UI_WeaponSwap", L"../Resources/UI/UI_WeaponSwap.png", 4, 2, false);
	//UI 가방
	_ImageManager->AddImage("UI_Bag", L"../Resources/UI/UI_Bag.png", false);

	//UI 돈 주머니
	_ImageManager->AddImage("moneyPoket", L"../Resources/UI/moneyBag.png", false);

	//UI 하트
	_ImageManager->AddImage("heart", L"../Resources/UI/heart.png", false);
	
	//UI PLAYER HP 프로그래스 바
	_ImageManager->AddFrameImage("hpBar1", L"../Resources/UI/hpBar.png", 1, 6, false);
	_ImageManager->AddFrameImage("hpBar2", L"../Resources/UI/hpBar.png", 1, 6, false);

	//UI BOSS HP 프로그래스 바
	_ImageManager->AddFrameImage("bossHpBar", L"../Resources/UI/bossHpBar.png", 1, 5, false);

	//UI 포션
	_ImageManager->AddImage("UI_Potion", L"../Resources/UI/UI_Potion.png", false);

	/**********************
			이팩트
	***********************/
	//소드 공격 이팩트
	_ImageManager->AddFrameImage("swordAtkEffect", L"../Resources/Effect/fx_hit_shortsword.png", 3, 1, false);
	//폭발 이팩트
	_ImageManager->AddFrameImage("boomEffect", L"../Resources/Effect/fx_boom.png", 10, 1, false);
	//활 공격 이팩트
	_ImageManager->AddFrameImage("bowAtkEffect", L"../Resources/Effect/fx_hit_bow.png", 3, 1, false);
	//보스 스크롤 이팩트
	_ImageManager->AddFrameImage("bossScrollEffect", L"../Resources/Effect/fx_BossScroll.png", 17, 1, false);


	/**********************
			아이템
	***********************/
	//나뭇가지 이미지
	_ImageManager->AddImage("item_brench", L"../Resources/Item/item_brench.png", false);
	
	//골렘 코어 이미지
	_ImageManager->AddImage("item_golemCore", L"../Resources/Item/item_golemCore.png", false);

	//레드 크리스탈 이미지
	_ImageManager->AddImage("item_redCristal", L"../Resources/Item/item_redCristal.png", false);


	/**********************
			 몬스터
	***********************/
	//플랑크톤 몬스터
	_ImageManager->AddFrameImage("planktonIMG", L"../Resources/Enemy/Plankton/Plankton.png", 10, 1, false);
	_ImageManager->AddFrameImage("planktonRedIMG", L"../Resources/Enemy/Plankton/Plankton_Red.png", 10, 1, false);
	_ImageManager->AddFrameImage("planktonWhiteIMG", L"../Resources/Enemy/Plankton/Plankton_White.png", 10, 1, false);
}

void MainScene::ChanInit()
{

	/**********************
	   오브젝트 매니저 등록
	***********************/
	//인벤토리 등록
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);
	
	//UI 무기 스왑 / UI 가방
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI 돈 주머니
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI 코인
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);
	
	//UI 포션
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//이팩트
	for (UINT i = 0; i < 30; ++i) 
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
	
	//몬스터
	_ObjectManager->AddObject(ObjectType::Object, new Plankton(Vector2(150, 250)));

	for (UINT i = 0; i < 50; ++i) 
	{
		_ObjectManager->AddObject(ObjectType::Object, new MoveItem("item_brench", Vector2(Math::Random(0, WinSizeX), Math::Random(0, WinSizeY))));
	}

	for (UINT i = 0; i < 50; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Object, new MoveItem("item_golemCore", Vector2(Math::Random(0, WinSizeX), Math::Random(0, WinSizeY))));
	}
	

}


void TownScene::ChanInit()
{	//인벤토리 등록
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI 무기 스왑 / UI 가방
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI 돈 주머니
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI 코인
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI 포션
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//이팩트
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}

//여기다가 1번 던전것들 올리시면 됩니다유
void Dungeon_Map_01::ChanInit()
{
	//인벤토리 등록
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI 무기 스왑 / UI 가방
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI 돈 주머니
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI 코인
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI 포션
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//이팩트
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}
//보스방 던전 여따가 넣을거 넣으심 됨요
void Dungeon_Map_Boss::ChanInit()
{
	//인벤토리 등록
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI 무기 스왑 / UI 가방
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI 돈 주머니
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI 포션
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//UI 코인
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//이팩트
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}

	//UI 보스 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new BossProgressBar);

	//보스 스크롤 이팩트
	_ObjectManager->AddObject(ObjectType::Effect, new BossEffect);
}



void Dungeon_Map_02::ChanInit()
{
	//인벤토리 등록
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI 무기 스왑 / UI 가방
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI 돈 주머니
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI 코인
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI 포션
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//이팩트
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}

void DungeonLobby::ChanInit()
{
	/**********************
	   오브젝트 매니저 등록
	***********************/
	//인벤토리 등록
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	////무브 아이템 등록 및 랜덤한 개수 맵 뿌리기
	//for (UINT i = 0; i < 70; ++i) 
	//{
	//	_ObjectManager->AddObject(ObjectType::Object, new MoveItem("brench", Vector2(Math::Random(0, WinSizeX), Math::Random(0, WinSizeY))));
	//}

	//UI 무기 스왑 / UI 가방
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI 프로그래스 바
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI 돈 주머니
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI 코인
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI 포션
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//이팩트
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}
