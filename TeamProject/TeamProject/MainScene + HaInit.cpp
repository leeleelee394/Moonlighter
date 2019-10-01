#include "stdafx.h"
#include "MainScene.h"
#include "TownScene.h"
#include "Dungeon_Map_01.h"
#include "Dungeon_Map_02.h"
#include "Dungeon_Map_Boss.h"
#include "Enemy.h"
#include "Player.h"
#include "BigSlime.h"
#include "Golem.h"
#include "Boss.h"
#include "Weeds.h"
#include "Rock.h"
#include "Program.h"
//#include "BigSlime.h"
void Program::LoadResourceHa()
{
	//------------보스 몸체 이미지-------------//
	//생성상태
	_ImageManager->AddFrameImage("create", L"../Resources/Enemy/Boss/CreateBoss.png", 32, 1, true);
	//사망상태
	_ImageManager->AddFrameImage("dead1", L"../Resources/Enemy/Boss/deadBoss1.png", 41, 1, true);
	_ImageManager->AddFrameImage("dead2", L"../Resources/Enemy/Boss/deadBoss2.png", 41, 1, true);
	//----------1번스킬----------//
	//손날리는 처음모션
	_ImageManager->AddFrameImage("hand_Shoot_First", L"../Resources/Enemy/Boss/Hand_Shoot_First.png", 20, 1, true);
	//손날리는 중간모션
	_ImageManager->AddFrameImage("hand_Shoot_second", L"../Resources/Enemy/Boss/Hand_Shoot_Second.png", 30, 1, true);
	//손날리는 마지막모션
	_ImageManager->AddFrameImage("hand_Shoot_Last", L"../Resources/Enemy/Boss/Hand_Shoot_Last.png", 11, 1, true);
	//손
	_ImageManager->AddFrameImage("hand", L"../Resources/Enemy/Boss/hand.png", 19, 1, true);

	//----------2번스킬------------//
	//보스 이미지
	_ImageManager->AddFrameImage("Rock_Shoot", L"../Resources/Enemy/Boss/rockShoot.png", 32, 1, true);

	//---------------3번 스킬----------------//
	//보스 이미지
	_ImageManager->AddFrameImage("FistShoot", L"../Resources/Enemy/Boss/FistShoot.png", 22, 3, true);

	//++++++++++++++++++++++++++++++++++++++++++보스 충돌 시 이미지++++++++++++++++++++++++++++++++++++++++++//
	//생성상태
	_ImageManager->AddFrameImage("create_Red", L"../Resources/Enemy/Boss/CreateBoss_Red.png", 32, 1, true);
	_ImageManager->AddFrameImage("create_White", L"../Resources/Enemy/Boss/CreateBoss_White.png", 32, 1, true);
	//사망상태1
	_ImageManager->AddFrameImage("dead1_Red", L"../Resources/Enemy/Boss/deadBoss1.png", 41, 1, true);
	_ImageManager->AddFrameImage("dead1_White", L"../Resources/Enemy/Boss/deadBoss1.png", 41, 1, true);
	//사망상태2
	_ImageManager->AddFrameImage("dead2_Red", L"../Resources/Enemy/Boss/deadBoss2.png", 41, 1, true);
	_ImageManager->AddFrameImage("dead2_White", L"../Resources/Enemy/Boss/deadBoss2.png", 41, 1, true);
	//손날리는 처음모션 피격
	_ImageManager->AddFrameImage("hand_Shoot_First_Red", L"../Resources/Enemy/Boss/hand_Shoot_First_Red.png", 20, 1, true);
	_ImageManager->AddFrameImage("hand_Shoot_First_White", L"../Resources/Enemy/Boss/hand_Shoot_First_White.png", 20, 1, true);
	//손날리는 중간모션 피격
	_ImageManager->AddFrameImage("hand_Shoot_second_Red", L"../Resources/Enemy/Boss/hand_Shoot_second_Red.png", 30, 1, true);
	_ImageManager->AddFrameImage("hand_Shoot_second_White", L"../Resources/Enemy/Boss/hand_Shoot_second_White.png", 30, 1, true);
	//손날리는 마지막모션 피격
	_ImageManager->AddFrameImage("hand_Shoot_Last_Red", L"../Resources/Enemy/Boss/hand_Shoot_Last_Red.png", 11, 1, true);
	_ImageManager->AddFrameImage("hand_Shoot_Last_White", L"../Resources/Enemy/Boss/hand_Shoot_Last_White.png", 11, 1, true);
	//손 피격
	_ImageManager->AddFrameImage("hand_Red", L"../Resources/Enemy/Boss/hand_Red.png", 19, 1, true);
	_ImageManager->AddFrameImage("hand_White", L"../Resources/Enemy/Boss/hand_White.png", 19, 1, true);
	//2번스킬 보스 이미지 피격
	_ImageManager->AddFrameImage("Rock_Shoot_Red", L"../Resources/Enemy/Boss/rockShoot_Red.png", 32, 1, true);
	_ImageManager->AddFrameImage("Rock_Shoot_White", L"../Resources/Enemy/Boss/rockShoot_White.png", 32, 1, true);
	//3번스킬 보스 이미지 피격
	_ImageManager->AddFrameImage("FistShoot_Red", L"../Resources/Enemy/Boss/FistShoot_Red.png", 22, 3, true);
	_ImageManager->AddFrameImage("FistShoot_White", L"../Resources/Enemy/Boss/FistShoot_White.png", 22, 3, true);
	//============빅 슬라임 이미지============//
	//생성
	_ImageManager->AddFrameImage("Create", L"../Resources/Enemy/MiddleBoss/CreateSlime.png", 37, 1, true);
	//무브
	_ImageManager->AddFrameImage("Move", L"../Resources/Enemy/MiddleBoss/SlimeMove.png", 8, 2, true);
	//공격
	_ImageManager->AddFrameImage("Attack", L"../Resources/Enemy/MiddleBoss/SlimeAttack.png", 22, 2, true);
	//사망
	_ImageManager->AddFrameImage("Dead", L"../Resources/Enemy/MiddleBoss/SlimeDead.png", 37, 1, true);
	//------------골렘 이미지-----------------//
	_ImageManager->AddFrameImage("GolemMove", L"../Resources/Enemy/Golem/GolemMove.png", 8, 4, true);
	_ImageManager->AddFrameImage("GolemMove_Red", L"../Resources/Enemy/Golem/GolemMove_Red.png", 8, 4, true);
	_ImageManager->AddFrameImage("GolemMove_White", L"../Resources/Enemy/Golem/GolemMove_White.png", 8, 4, true);
	_ImageManager->AddFrameImage("GolemAttack", L"../Resources/Enemy/Golem/GolemAttack.png", 13, 4, true);
	_ImageManager->AddFrameImage("GolemAttack_Red", L"../Resources/Enemy/Golem/GolemAttack_Red.png", 13, 4), true;
	_ImageManager->AddFrameImage("GolemAttack_White", L"../Resources/Enemy/Golem/GolemAttack_White.png", 13, 4, true);

	//------------------잡초---------------------//
	_ImageManager->AddFrameImage("weeds", L"../Resources/Enemy/Weeds/Weeds.png", 12, 1, true);
	_ImageManager->AddFrameImage("weeds_Red", L"../Resources/Enemy/Weeds/Weeds_Red.png", 12, 1, true);
	_ImageManager->AddFrameImage("weeds_White", L"../Resources/Enemy/weeds/Weeds_White.png", 12, 1, true);

	//--------------그림자-----------------//
	_ImageManager->AddImage("shadow", L"../Resources/Object/Shadow.png");

	//---------------돌--------------------//
	_ImageManager->AddImage("rock", L"../Resources/Enemy/Boss/Boss_Rock0.png", true);

	//====================================사운드=====================================//
	_SoundManager->AddSound("golemHit", "../Sounds/golem_hit.wav", false, false);
	_SoundManager->AddSound("enemyDeath", "../Sounds/enemy_death.wav", false, false);
	_SoundManager->AddSound("Weeds_Hit", "../Sounds/weed_hit.wav", false, false);
	_SoundManager->AddSound("bossDeath", "../Sounds/bossDeath.wav", false, false);
	_SoundManager->AddSound("bossCreate", "../Sounds/GolemKingWakeUp.wav", false, false);
	_SoundManager->AddSound("GolemKingHandCrush", "../Sounds/GolemKingHandCrush.wav", false, false);
	_SoundManager->AddSound("GolemKingRockHitGround", "../Sounds/GolemKingRockHitGround.wav", false, false);
	_SoundManager->AddSound("GolemKingRoom", "../Sounds/GolemKingRoom.mp3", true, true);
	// 보스슬라임 나와있는 동안 계속 재생되는 사운드
	_SoundManager->AddSound("dungeon_wanderer_idle", "../Sounds/dungeon_wanderer_idle.wav", false, false);
	// 보스슬라임 등장 사운드
	_SoundManager->AddSound("dungeon_wanderer_appears", "../Sounds/dungeon_wanderer_appears.wav", false, false);
	// 보스 슬라임 공격 사운드
	_SoundManager->AddSound("dungeon_wanderer_attack", "../Sounds/dungeon_wanderer_attack.wav", false, false);
	//골렘 공격 사운드
	_SoundManager->AddSound("golem_dungeon_golem_crash", "../Sounds/golem_dungeon_golem_crash.wav", false, false);

}
//여기서 필요한 리소스 로드 및 클래스 할당하시면 됩니다. 
//Init은 ㄴㄴ new 하고 ObjectManager에 추가 ㅇㅇ
void MainScene::HaInit()
{
	_ObjectManager->AddObject(ObjectType::Object, new Golem(Vector2(100, 100)));
	_ObjectManager->AddObject(ObjectType::Object, new Weeds(Vector2(300, 100)));
	_ObjectManager->AddObject(ObjectType::Object, new BigSlime(Vector2(100, 500)));
}

void Dungeon_Map_01::HaInit()
{
	_ObjectManager->AddObject(ObjectType::Object, new Golem(Vector2(100, 100)));
	_ObjectManager->AddObject(ObjectType::Object, new Golem(Vector2(900, 100)));
	_ObjectManager->AddObject(ObjectType::Object, new Weeds(Vector2(100, 400)));
	_ObjectManager->AddObject(ObjectType::Object, new Weeds(Vector2(900, 400)));

	_ObjectManager->AddObject(ObjectType::Object, new BigSlime(Vector2(WinSizeX / 2, WinSizeY / 2)));
}

void Dungeon_Map_02::HaInit()
{
	_ObjectManager->AddObject(ObjectType::Object, new BigSlime(Vector2(400, 320)));
	_ObjectManager->AddObject(ObjectType::Object, new BigSlime(Vector2(900, 600)));
}

void Dungeon_Map_Boss::HaInit()
{
	_ObjectManager->AddObject(ObjectType::Object, new Boss());
}

void TownScene::HaInit()
{
}

