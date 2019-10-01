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
//���⼭ �ʿ��� ���ҽ� �ε� �� Ŭ���� �Ҵ��Ͻø� �˴ϴ�. 
//Init�� ���� new �ϰ� ObjectManager�� �߰� ����


void Program::LoadResourceChan()
{
	/**********************
		     ����
	***********************/
	_SoundManager->AddSound("SelectTarget", "../Sounds/gui_selector_movement.wav", false, false);
	_SoundManager->AddSound("InvenOpenSound", "../Sounds/gui_inventory_mirror_action.wav", false, false);


	/**********************
			   UI
	***********************/
	//���� �κ��丮 �̹��� 
	_ImageManager->AddImage("InventoryWindow", L"../Resources/UI/newInventory.png", false);
	_ImageManager->AddImage("InvenGlasses", L"../Resources/UI/Bag_Mirror.png", false);

	//UI ����
	_ImageManager->AddImage("Coin", L"../Resources/UI/coin.png", false);

	//UI ���� ����
	_ImageManager->AddFrameImage("UI_WeaponSwap", L"../Resources/UI/UI_WeaponSwap.png", 4, 2, false);
	//UI ����
	_ImageManager->AddImage("UI_Bag", L"../Resources/UI/UI_Bag.png", false);

	//UI �� �ָӴ�
	_ImageManager->AddImage("moneyPoket", L"../Resources/UI/moneyBag.png", false);

	//UI ��Ʈ
	_ImageManager->AddImage("heart", L"../Resources/UI/heart.png", false);
	
	//UI PLAYER HP ���α׷��� ��
	_ImageManager->AddFrameImage("hpBar1", L"../Resources/UI/hpBar.png", 1, 6, false);
	_ImageManager->AddFrameImage("hpBar2", L"../Resources/UI/hpBar.png", 1, 6, false);

	//UI BOSS HP ���α׷��� ��
	_ImageManager->AddFrameImage("bossHpBar", L"../Resources/UI/bossHpBar.png", 1, 5, false);

	//UI ����
	_ImageManager->AddImage("UI_Potion", L"../Resources/UI/UI_Potion.png", false);

	/**********************
			����Ʈ
	***********************/
	//�ҵ� ���� ����Ʈ
	_ImageManager->AddFrameImage("swordAtkEffect", L"../Resources/Effect/fx_hit_shortsword.png", 3, 1, false);
	//���� ����Ʈ
	_ImageManager->AddFrameImage("boomEffect", L"../Resources/Effect/fx_boom.png", 10, 1, false);
	//Ȱ ���� ����Ʈ
	_ImageManager->AddFrameImage("bowAtkEffect", L"../Resources/Effect/fx_hit_bow.png", 3, 1, false);
	//���� ��ũ�� ����Ʈ
	_ImageManager->AddFrameImage("bossScrollEffect", L"../Resources/Effect/fx_BossScroll.png", 17, 1, false);


	/**********************
			������
	***********************/
	//�������� �̹���
	_ImageManager->AddImage("item_brench", L"../Resources/Item/item_brench.png", false);
	
	//�� �ھ� �̹���
	_ImageManager->AddImage("item_golemCore", L"../Resources/Item/item_golemCore.png", false);

	//���� ũ����Ż �̹���
	_ImageManager->AddImage("item_redCristal", L"../Resources/Item/item_redCristal.png", false);


	/**********************
			 ����
	***********************/
	//�ö�ũ�� ����
	_ImageManager->AddFrameImage("planktonIMG", L"../Resources/Enemy/Plankton/Plankton.png", 10, 1, false);
	_ImageManager->AddFrameImage("planktonRedIMG", L"../Resources/Enemy/Plankton/Plankton_Red.png", 10, 1, false);
	_ImageManager->AddFrameImage("planktonWhiteIMG", L"../Resources/Enemy/Plankton/Plankton_White.png", 10, 1, false);
}

void MainScene::ChanInit()
{

	/**********************
	   ������Ʈ �Ŵ��� ���
	***********************/
	//�κ��丮 ���
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);
	
	//UI ���� ���� / UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI �� �ָӴ�
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);
	
	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//����Ʈ
	for (UINT i = 0; i < 30; ++i) 
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
	
	//����
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
{	//�κ��丮 ���
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI ���� ���� / UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI �� �ָӴ�
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//����Ʈ
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}

//����ٰ� 1�� �����͵� �ø��ø� �˴ϴ���
void Dungeon_Map_01::ChanInit()
{
	//�κ��丮 ���
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI ���� ���� / UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI �� �ָӴ�
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//����Ʈ
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}
//������ ���� ������ ������ ������ �ʿ�
void Dungeon_Map_Boss::ChanInit()
{
	//�κ��丮 ���
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI ���� ���� / UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI �� �ָӴ�
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//����Ʈ
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}

	//UI ���� ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new BossProgressBar);

	//���� ��ũ�� ����Ʈ
	_ObjectManager->AddObject(ObjectType::Effect, new BossEffect);
}



void Dungeon_Map_02::ChanInit()
{
	//�κ��丮 ���
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	//UI ���� ���� / UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI �� �ָӴ�
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//����Ʈ
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}

void DungeonLobby::ChanInit()
{
	/**********************
	   ������Ʈ �Ŵ��� ���
	***********************/
	//�κ��丮 ���
	_ObjectManager->AddObject(ObjectType::UI, new Inventory);

	////���� ������ ��� �� ������ ���� �� �Ѹ���
	//for (UINT i = 0; i < 70; ++i) 
	//{
	//	_ObjectManager->AddObject(ObjectType::Object, new MoveItem("brench", Vector2(Math::Random(0, WinSizeX), Math::Random(0, WinSizeY))));
	//}

	//UI ���� ���� / UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIWeaponAndBag);

	//UI ���α׷��� ��
	_ObjectManager->AddObject(ObjectType::UI, new ProgressBar);

	//UI �� �ָӴ�
	_ObjectManager->AddObject(ObjectType::UI, new UIMoneyPoket);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UICoin);

	//UI ����
	_ObjectManager->AddObject(ObjectType::UI, new UIPotion);

	//����Ʈ
	for (UINT i = 0; i < 30; ++i)
	{
		_ObjectManager->AddObject(ObjectType::Effect, new Effect);
	}
}