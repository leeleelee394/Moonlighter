#include "stdafx.h"
#include "MainScene.h"
#include "TownScene.h"
#include "Dungeon_Map_01.h"
#include "Dungeon_Map_Boss.h"
#include "Dungeon_Map_02.h"
#include "DungeonLobby.h"
#include "TestPlayer.h"
#include "TestInventory.h"
#include "SceneChangeObject.h"

#include "StaticObject.h"
#include "DebugCollider.h"
#include "DebugObject.h"
#include "FrameObject.h"
#include "Tree.h"
#include "Program.h"
#include "Player.h"
#include "DungeonGate.h"
#include "DebugFrameObject.h"
#include "Animation.h"
#include "OilLamp.h"
#include "PointLight.h"
#include "OilLampLight.h"
#include "LightParticleSystem.h"
#include "Cloud.h"

void Program::LoadResourceSoon()
{
	_ImageManager->AddFrameImage("door", L"../Resources/Object/door.png", 5, 1);
	_ImageManager->AddFrameImage("tree", PathResources(L"Object/tree.png"), 4, 1);
	_ImageManager->AddFrameImage("RivalFauntain", PathResources(L"Object/RivalFauntain.png"), 9, 1);
	_ImageManager->AddFrameImage("dungeonLobby_goInDungeon", PathResources(L"Object/dungeonLobby_goInDungeon.png"), 21, 1);
	_ImageManager->AddFrameImage("dungeonLobby_goOutDungeon", PathResources(L"Object/dungeonLobby_goOutDungeon.png"), 56, 1);
	_ImageManager->AddFrameImage("dungeonLobby_doorOpen", PathResources(L"Object/dungeonLobby_doorOpen.png"), 16, 1);
	_ImageManager->AddFrameImage("DungeonGateOpenIdle", PathResources(L"Object/DungeonGateOpenIdle.png"), 6, 1);
	_ImageManager->AddFrameImage("dungeonLobby_bottomLeftTent", PathResources(L"Object/dungeonLobby_bottomLeftTent.png"), 8, 1);

	_ImageManager->AddImage("LeftDoor", PathResources(L"Scene/door_left.png"));
	_ImageManager->AddImage("RightDoor", PathResources(L"Scene/door_right.png"));
	_ImageManager->AddImage("TitleImage", PathResources(L"Scene/logo.png"));
	_ImageManager->AddImage("Selector", PathResources(L"Scene/Selector.png"));
	_ImageManager->AddFrameImage("door_light", PathResources(L"Scene/door_light.png"), 30, 1, false);

	_ImageManager->AddImage("Dungeon_Lobby", PathResources(L"Scene/Dungeon_Lobby.png"));
	_ImageManager->LoadFolder("../Resources/Object/", "*.png");

	_SoundManager->AddSound("DoorOpen", PathSounds("door.wav"));
	_SoundManager->AddSound("introBGM", PathSounds("introBGM.mp3"), true, true);
	_SoundManager->AddSound("townBGM", PathSounds("townBGM.mp3"), true, true);
	_SoundManager->AddSound("poolRoomBGM", PathSounds("poolRoomBGM.mp3"), true, true);
	_SoundManager->AddSound("dungeonBGM", PathSounds("dungeonBGM.mp3"), true, true);
	_SoundManager->AddSound("dungeon_entrance_slime_door_absorb", PathSounds("dungeon_entrance_slime_door_absorb.wav"));
	_SoundManager->AddSound("dungeon_entrance_slime_door_spit", PathSounds("dungeon_entrance_slime_door_spit.wav"));

	_SoundManager->SetMusicVolume(0.1f);

}


void MainScene::SoonInit()
{

}
void Dungeon_Map_01::SoonInit()
{

}


void Dungeon_Map_02::SoonInit()
{
}

void Dungeon_Map_Boss::SoonInit()
{
	//_ObjectManager->AddObject(ObjectType::UI, new DebugCollider);

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({0,1340,2101,1422}));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 0,0,73,1422 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 0,0,2101,84 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 2020,0,2096,1420 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 249,291,1852,372 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1344,1258,2014,1337 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1582,1115,2013,1280 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1743,944,2016,1340 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1834,655,2018,966 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1961,533,2020,687 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 465,1247,848,1339 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 69,1086,482,1336 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 80,947,402,1102 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 61,750,264,993 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 71,568,148,798 }));
}
void TownScene::SoonInit()
{
	StaticObject* tempObject = nullptr;
	tempObject = new StaticObject("build_Retaile", Vector2(20, 290));
	tempObject->SetCollisionRect({ 314,312,650,676 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Enchant", Vector2(50, 1298));
	tempObject->SetCollisionRect({ 85,1580,641,2050 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Top1", Vector2(1092, 236));
	tempObject->SetCollisionRect({ 1100,306,1417,622 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Shop", Vector2(1620, 194));
	tempObject->SetCollisionRect({ 1619,303,2148,626 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Board", Vector2(1574, 1078));
	tempObject->SetCollisionRect({ 1599,1100,1759,1214 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Bottom1", Vector2(762, 1652));
	tempObject->SetCollisionRect({ 909,1678,1229,1981 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Bottom2", Vector2(1725, 1545));
	tempObject->SetCollisionRect({ 1737,1619,2057,1927 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Forge", Vector2(2298, 786));
	tempObject->SetCollisionRect({ 2328,876,2934,1270 });
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	tempObject = new StaticObject("build_Bottom3", Vector2(2369, 1405));
	_ObjectManager->AddObject(ObjectType::Object, tempObject);

	FrameObject* rival = new FrameObject("RivalFauntain", Vector2(467, 923));
	rival->SetPivot(Pivot::CENTER);
	rival->SetSize(Vector2(128, 128));
	rival->SetCollisionRect({ 440,894,497,952 });
	_ObjectManager->AddObject(ObjectType::Object, rival);

	Tree* tempFrameObject = nullptr;

	tempFrameObject = new Tree(Vector2(912, 306));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(912, 606));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(442, 306));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(990, 1000));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(990, 1460));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(2014, 1000));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(2014, 1460));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(2368, 556));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	tempFrameObject = new Tree(Vector2(2201, 1868));
	_ObjectManager->AddObject(ObjectType::Object, tempFrameObject);

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ -10, -10, (LONG)_townBackgroundImage->GetWidth() + 10, 0 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ -10, -10, 0, (LONG)_townBackgroundImage->GetHeight() }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ -10,(LONG)_townBackgroundImage->GetHeight() ,
		(LONG)_townBackgroundImage->GetWidth() + 10 ,(LONG)_townBackgroundImage->GetHeight() + 10 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ (LONG)_townBackgroundImage->GetWidth(),0,
		(LONG)_townBackgroundImage->GetWidth() + 10, (LONG)_townBackgroundImage->GetHeight() + 10 }));

	_ObjectManager->AddObject(ObjectType::Object, new SceneChangeObject({ 703,0,850,12 }, []() 
	{
		//1241,1997
		Database::Get()->AddVector2Data("PlayerPosition", Vector2(1241, 1997));
		_SceneManager->LoadSceneByLoading("DungeonLobby");
	}));

	//_ObjectManager->AddObject(ObjectType::UI, new DebugCollider);

	//1310,1220
	OilLamp* oilLamp = new OilLamp("Village_OilLamps_1", Vector2(2310, 1220));
	_ObjectManager->AddObject(ObjectType::Object, oilLamp);
	//22310,1406
	_ObjectManager->AddObject(ObjectType::Object, new OilLamp("Village_OilLamps_1", Vector2(2310, 1406)));
	//1524,652
	_ObjectManager->AddObject(ObjectType::Object, new OilLamp("Village_OilLamps_2", Vector2(1524, 652)));
	//2216,652
	_ObjectManager->AddObject(ObjectType::Object, new OilLamp("Village_OilLamps_2", Vector2(2216, 652)));

	//1524,552
	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(1524, 552)));
	//2216,554
	_ObjectManager->AddObject(ObjectType::UpObject,new OilLampLight(Vector2(2216,554)));

	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(2288, 1312)));
	//1127
	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(2288, 1127)));
	//1588,1161
	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(1588,1161),30.f));
	//1761,1161
	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(1761, 1161), 30.f));

	//1244,892
	_ObjectManager->AddObject(ObjectType::Object, new OilLamp("Village_OilLamps_2", Vector2(1244, 892)));
	//1468
	_ObjectManager->AddObject(ObjectType::Object, new OilLamp("Village_OilLamps_2", Vector2(1244, 1468)));
	//1244,790
	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(1244, 790)));
	//1242,1367
	_ObjectManager->AddObject(ObjectType::UpObject, new OilLampLight(Vector2(1242, 1367)));

	_ObjectManager->AddObject(ObjectType::UpObject, new LightParticleSystem(Vector2(1242, 1000)));

	for (UINT i = 0; i < 10; ++i)
	{
		Vector2 randomPos = Vector2(Math::Random(0.f, this->_townBackgroundImage->GetSize().x), Math::Random(0.f, this->_townBackgroundImage->GetSize().y));
		_ObjectManager->AddObject(ObjectType::UpObject, new Cloud(randomPos));
	}
}


void DungeonLobby::SoonInit()
{
	//_ObjectManager->AddObject(ObjectType::UI, new DebugObject("dungeonLobby_bottomRight2Rock"));
	//_ObjectManager->AddObject(ObjectType::UI, new DebugCollider());

	_ObjectManager->AddObject(ObjectType::Object, new Player(Vector2(1000, 1000)));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 578,1172,1180,1195 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1307,1168,1890,1195 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 577,850,618,1186 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 576,848,874,910 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1849,847,1889,1192 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1608,843,1879,929 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1608,527,1642,927 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1422,527,1642,637 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1385,182,1422,637 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1053,184,1413,301 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1051,184,1091,633 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 831,526,874,886 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 831,524,1075,634 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 828,761,906,816 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 935,668,990,699 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ 1489,664,1538,689 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ -10, -10, (LONG)_background->GetWidth() + 10, 0 }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ -10, -10, 0, (LONG)_background->GetHeight() }));
	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ -10,(LONG)_background->GetHeight() ,
		(LONG)_background->GetWidth() + 10 ,(LONG)_background->GetHeight() + 10 }));

	_ObjectManager->AddObject(ObjectType::Object, new GameObject({ (LONG)_background->GetWidth(),0,
		(LONG)_background->GetWidth() + 10, (LONG)_background->GetHeight() + 10 }));

	//1020,409
	_ObjectManager->AddObject(ObjectType::UpObject, new StaticObject("dungeonLobby_top3LongBoard", Vector2(1020, 410)));
	//791,458
	_ObjectManager->AddObject(ObjectType::UpObject, new StaticObject("dungeonLobby_topRope", Vector2(792, 458)));
	//563,988
	//_ObjectManager->AddObject(ObjectType::UpObject, new StaticObject("dungeonLobby_bottomLeftTent", Vector2(563, 988)));
	//1175,1076
	_ObjectManager->AddObject(ObjectType::UpObject, new StaticObject("dungeonLobby_entryStructureFlag", Vector2(1176, 1076)));

	StaticObject* temp = new StaticObject("dungeonLobby_entryStructureLeft", Vector2(1059, 1058));
	temp->SetCollisionRect({ 1059,1169,1181,1224 });
	_ObjectManager->AddObject(ObjectType::Object, temp);
	//1307,1058
	temp = new StaticObject("dungeonLobby_entryStructureRight", Vector2(1308, 1058));
	temp->SetCollisionRect({ 1306,1170,1428,1224 });
	_ObjectManager->AddObject(ObjectType::Object, temp);

	//1333,1247
	temp = new StaticObject("forgeBoard", Vector2(1333, 1247));
	_ObjectManager->AddObject(ObjectType::Object, temp);

	//1042,1308
	temp = new StaticObject("dungeonLobby_topLeft3Rock", Vector2(1042, 1308));
	_ObjectManager->AddObject(ObjectType::Object, temp);

	temp = new StaticObject("dungeonLobby_middleLeftRock", Vector2(906, 1792));
	_ObjectManager->AddObject(ObjectType::Object, temp);

	temp = new StaticObject("dungeonLobby_middleRighttRock", Vector2(1366, 1334));
	_ObjectManager->AddObject(ObjectType::Object, temp);

	temp = new StaticObject("dungeonLobby_middleRighttRock", Vector2(1336, 1638));
	_ObjectManager->AddObject(ObjectType::Object, temp);

	temp = new StaticObject("dungeonLobby_bottomRight2Rock", Vector2(1422, 1872));
	_ObjectManager->AddObject(ObjectType::Object, temp);

	_ObjectManager->AddObject(ObjectType::Object, new DungeonGate);

	_ObjectManager->AddObject(ObjectType::Object, new SceneChangeObject({ 1182,2080,1328,2102 }, []()
	{
		//770,51
		_Database->AddVector2Data("PlayerPosition", Vector2(770, 51));
		_SceneManager->LoadSceneByLoading("TownScene");
	}));

	//_ObjectManager->AddObject(ObjectType::UI, new DebugFrameObject("dungeonLobby_bottomLeftTent"));

	FrameObject* tent = new FrameObject("dungeonLobby_bottomLeftTent", Vector2(518, 985));
	tent->SetSize(Vector2(256.f, 256.f));
	tent->SetPivot(Pivot::LEFT_TOP);
	tent->GetAnimation()->SetFrameUpdateTime(0.1f);
	_ObjectManager->AddObject(ObjectType::UpObject, tent);

	for (UINT i = 0; i < 10; ++i)
	{
		Vector2 randomPos = Vector2(Math::Random(0.f, this->_background->GetSize().x), Math::Random(0.f, this->_background->GetSize().y));
		_ObjectManager->AddObject(ObjectType::UpObject, new Cloud(randomPos));
	}
}
