#include "../../External/nlohmann/json.hpp"
#include "../Object/Actor/Player/Player.h"
#include "../Object/Actor/Enemy/SlenderManEnemy.h"
#include "../Object/Actor/Enemy/GirlEnemy.h"
#include "../Object/Actor/Enemy/MannequinEnemy.h"
#include "../Object/Actor/Enemy/ZombieEnemy.h"
#include "../Object/Map/Map.h"
#include "../Object/Props/LightBulb.h"
#include "../Object/Props/Door.h"
#include "../Object/Props/Key.h"
#include "../Object/Props/Padlock.h"
#include "../Object/Props/Locker.h"
#include "../Object/Map/Wave/Wave.h"
#include "../Object/ObjectManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/Component/Actor/Algorithm/AStarChaser.h"
#include "../Object/Component/Actor/Algorithm/RouteWalker.h"
#include "../Object/Component/Actor/AnimationController.h"
#include "../Object/Component/Controller/SmoothRotateController.h"
#include "../Camera/Camera.h"
#include "ObjectSpawner.h"

namespace
{
	// オブジェクト設定
	const string OBJECT_KEY_PLAYER = "PLAYER";
	const Transform OBJECT_TRANSFORM_PLAYER = { Position3D(50.0f ,30.0f ,1450.0f),Quaternion::Euler({ 0.0f,Deg2Radian(180.0f),0.0f }) };

	// マップ
	const string OBJECT_KEY_MAP = "MAP";

	// 波
	const string OBJECT_KEY_WAVE = "WAVE";
	const vector<pair<Position3D, Size2D>> OBJECT_TRANSFORM_WAVE =
	{
		{Position3D(50.0f,-90.0f,-900.0f),		Size2D(1000.0f,750.0f)},
		{Position3D(2000.0f,-90.0f,1450.0f),	Size2D(1000.0f,750.0f)},
		{Position3D(-2750.0f,-90.0f,-950.0f),	Size2D(1000.0f,750.0f)}
	};

	// ロッカー
	const string OBJECT_KEY_LOCKER = "LOCKER";
	const Transform OBJECT_TRANSFORM_LOCKER[] =
	{
		{Position3D(990.0f,  -85.0f,240.0f),	Quaternion::Euler({0.0f,Deg2Radian(-90.0f),0.0f}),	Scale3D(1.2f,1.2f,1.2f)},
		{Position3D(-230.0f, -85.0f,-830.0f),	Quaternion::Euler({0.0f,Deg2Radian(-90.0f),0.0f}),	Scale3D(1.2f,1.2f,1.2f)},
		{Position3D(-1680.0f,-85.0f,-1480.0f),	Quaternion::Euler({0.0f,0.0f,0.0f}),				Scale3D(1.2f,1.2f,1.2f)},
		{Position3D(1940.0f, -85.0f,-1080.0f),	Quaternion::Euler({0.0f,0.0f,0.0f}),				Scale3D(1.2f,1.2f,1.2f)},
		{Position3D(1780.0f, -85.0f,1980.0f),	Quaternion::Euler({0.0f,Deg2Radian(180.0f),0.0f}),	Scale3D(1.2f,1.2f,1.2f)},
		{Position3D(320.0f,  -85.0f,1540.0f),	Quaternion::Euler({0.0f,Deg2Radian(90.0f),0.0f}),	Scale3D(1.2f,1.2f,1.2f)},
		{Position3D(-2450.0f,-85.0f,850.0f),	Quaternion::Euler({0.0f,Deg2Radian(90.0f),0.0f}),	Scale3D(1.2f,1.2f,1.2f)},
	};

	// 鍵
	const string OBJECT_KEY_KEY = "KEY";
	const Transform OBJECT_TRANSFORM_KEY[] =
	{
		{Position3D(2450.0f, -85.0f,1450.0f),	Quaternion::Euler({0.0f,Deg2Radian(45.0f),0.0f}),	Scale3D(3.0f,3.0f,3.0f)},
		{Position3D(-3200.0f,-85.0f,200.0f),	Quaternion::Euler({0.0f,Deg2Radian(70.0f),0.0f}),	Scale3D(3.0f,3.0f,3.0f)},
		{Position3D(-350.0f, -85.0f,4250.0f),	Quaternion::Euler({0.0f,Deg2Radian(30.0f),0.0f}),	Scale3D(3.0f,3.0f,3.0f)},
	};

	// 南京錠
	const string OBJECT_KEY_PADLOCK = "PADLOCK";
	const Transform OBJECT_TRANSFORM_PADLOCK[] =
	{
		{Position3D(30.0f,	40.0f,-730.0f),	Quaternion::Euler({0.0f,Deg2Radian(0.0f),0.0f}),	Scale3D(2.0f,2.0f,2.0f)},
		{Position3D(50.0f, 70.0f,-730.0f),	Quaternion::Euler({0.0f,Deg2Radian(0.0f),0.0f}),	Scale3D(2.0f,2.0f,2.0f)},
		{Position3D(70.0f,	40.0f,-730.0f),	Quaternion::Euler({0.0f,Deg2Radian(0.0f),0.0f}),	Scale3D(2.0f,2.0f,2.0f)},
	};

	// ドア
	const string OBJECT_KEY_DOOR = "DOOR";
	const Transform OBJECT_TRANSFORM_DOOR = { Position3D(50.0f,-105.0f,-755.0f),	Quaternion::Euler({0.0f,0.0f,0.0f}) };

	// エネミー
	const string OBJECT_KEY_ENEMY_ALIEN = "ENEMY_ALIEN";
	const string OBJECT_KEY_ENEMY_SLENDERMAN = "ENEMY_SLENDERMAN";
	const string OBJECT_KEY_ENEMY_MANNEQUIN = "ENEMY_MANNEQUIN";
	const string OBJECT_KEY_ENEMY_GIRL = "ENEMY_GIRL";
	const string OBJECT_KEY_ENEMY_ZOMBIE = "ENEMY_ZOMBIE";

	const string ANIMATION_KEY_ENEMY_WALK = "WALK";
	const string ANIMATION_KEY_ENEMY_RUN = "RUN";
}

ObjectSpawner::ObjectSpawner()
{
	LoadJsonData();
}

ObjectSpawner::~ObjectSpawner()
{
}

void ObjectSpawner::Terminate()
{
}

void ObjectSpawner::ObjectSpawn(int level)
{

	auto& objMng = ObjectMng;

	// プレイヤー
	shared_ptr<Object> object = make_shared<Player>(OBJECT_KEY_PLAYER);

	object->SetTransform(OBJECT_TRANSFORM_PLAYER);

	objMng.AddObject(object);


	// マップ
	object = make_shared<Map>(OBJECT_KEY_MAP);
	objMng.AddObject(object);


	// 波
	object = make_shared<Wave>(OBJECT_KEY_WAVE, OBJECT_TRANSFORM_WAVE);
	objMng.AddObject(object);

	// ロッカー
	for (int i = 0; i < _countof(OBJECT_TRANSFORM_LOCKER); i++)
	{
		object = make_shared<Locker>(OBJECT_KEY_LOCKER + to_string(i));
		objMng.AddObject(object);
		object->SetTransform(OBJECT_TRANSFORM_LOCKER[i]);
	}

	// キー
	for (int i = 0; i < _countof(OBJECT_TRANSFORM_KEY); i++)
	{
		EVENT_ID getEventID = static_cast<EVENT_ID>(static_cast<int>(EVENT_ID::GET_KEY_A) + i);
		EVENT_ID useEventID = static_cast<EVENT_ID>(static_cast<int>(EVENT_ID::USE_KEY_A) + i);
		object = make_shared<Key>(OBJECT_KEY_KEY + to_string(i), getEventID, useEventID);
		objMng.AddObject(object);
		object->SetTransform(OBJECT_TRANSFORM_KEY[i]);
	}

	// 南京錠
	for (int i = 0; i < _countof(OBJECT_TRANSFORM_PADLOCK); i++)
	{
		EVENT_ID eventID = static_cast<EVENT_ID>(static_cast<int>(EVENT_ID::UNLOCK_PADLOCK_A) + i);
		object = make_shared<Padlock>(OBJECT_KEY_PADLOCK + to_string(i), eventID);
		objMng.AddObject(object);
		object->SetTransform(OBJECT_TRANSFORM_PADLOCK[i]);
	}

	// ドア
	object = make_shared<Door>(OBJECT_KEY_DOOR, static_cast<int>(_countof(OBJECT_TRANSFORM_PADLOCK)));
	objMng.AddObject(object);
	object->SetPosition({ 50.0f,-105.0f,-755.0f });

	objMng.InitObjects();

	EnemySpawn(level);
}

void ObjectSpawner::EnemySpawn(int level)
{
	int levelKey = level;

	if (!levelData_.contains(levelKey))return;

	const auto& data = levelData_.at(levelKey);

	float speedRate = data.speedRate;
	float searchRate = data.searchRate;
	int alienNum = data.alienNum;
	int girlNum = data.girlNum;
	int mannequinNum = data.mannequinNum;
	int slenderManNum = data.slenderManNum;
	int zombieNum = data.zombieNum;

	auto& objMng = ObjectMng;
	shared_ptr<Object> object = nullptr;


	// スレンダーマン
	for (int i = 0; i < slenderManNum;i++)
	{
		if (static_cast<int>(slenderManData_.size()) <= i)break;

		const auto& enemyData = slenderManData_.at(i);

		object = make_shared<SlenderManEnemy>(OBJECT_KEY_ENEMY_SLENDERMAN + to_string(i));
		object->Init();
		objMng.AddObject(object);

		object->SetTransform(enemyData.spawnTransform);

		auto& animationCon = object->GetComponent<AnimationController>();
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_WALK);
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_RUN);

		auto& stateMachine = object->GetComponent<StateMachine<SlenderManEnemy::STATE>>();
		auto& chaser = stateMachine.GetState(SlenderManEnemy::STATE::CHASE).GetComponent<AStarChaser>();

		chaser.SetSpeed(enemyData.runSpeed * speedRate);
		chaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());
		chaser.SetTargetTransform(objMng.GetSpecificObject(OBJECT_KEY_PLAYER)->GetComponent<Transform>());

		auto& retChaser = stateMachine.GetState(SlenderManEnemy::STATE::RETURN).GetComponent<AStarChaser>();
		retChaser.SetSpeed(enemyData.walkSpeed * speedRate);
		retChaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());
		

		auto& walker = stateMachine.GetState(SlenderManEnemy::STATE::DEFAULT).GetComponent<RouteWalker>();
		walker.SetSpeed(enemyData.walkSpeed * speedRate);
		walker.InitStart(0);

	}

	// ガール
	for (int i = 0; i < girlNum; i++)
	{
		if (static_cast<int>(girlData_.size()) <= i)break;

		const auto& enemyData = girlData_.at(i);

		object = make_shared<GirlEnemy>(OBJECT_KEY_ENEMY_GIRL + to_string(i));
		object->Init();
		objMng.AddObject(object);

		object->SetTransform(enemyData.spawnTransform);


		auto& rotateCon = object->GetComponent<SmoothRotateController>();
		rotateCon.SetGoalQuaternion(enemyData.spawnTransform.qua);

		auto& animationCon = object->GetComponent<AnimationController>();
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_WALK);
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_RUN);

		auto& stateMachine = object->GetComponent<StateMachine<GirlEnemy::STATE>>();
		auto& chaser = stateMachine.GetState(GirlEnemy::STATE::CHASE).GetComponent<AStarChaser>();

		chaser.SetSpeed(enemyData.runSpeed * speedRate);
		chaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());
		chaser.SetTargetTransform(objMng.GetSpecificObject(OBJECT_KEY_PLAYER)->GetComponent<Transform>());

		auto& retChaser = stateMachine.GetState(GirlEnemy::STATE::RETURN).GetComponent<AStarChaser>();
		retChaser.SetSpeed(enemyData.walkSpeed * speedRate);
		retChaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());


		auto& walker = stateMachine.GetState(GirlEnemy::STATE::DEFAULT).GetComponent<RouteWalker>();
		walker.SetSpeed(enemyData.walkSpeed * speedRate);
	}


	// マネキン
	for (int i = 0; i < mannequinNum; i++)
	{
		if (static_cast<int>(mannequinData_.size()) <= i)break;

		const auto& enemyData = mannequinData_.at(i);

		object = make_shared<MannequinEnemy>(OBJECT_KEY_ENEMY_MANNEQUIN + to_string(i));
		object->SetTransform(enemyData.spawnTransform);
		object->Init();
		objMng.AddObject(object);

		auto& rotateCon = object->GetComponent<SmoothRotateController>();
		rotateCon.SetGoalQuaternion(enemyData.spawnTransform.qua);

		auto& animationCon = object->GetComponent<AnimationController>();
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_WALK);
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_RUN);

		auto& stateMachine = object->GetComponent<StateMachine<MannequinEnemy::STATE>>();
		auto& chaser = stateMachine.GetState(MannequinEnemy::STATE::CHASE_MOVE).GetComponent<AStarChaser>();

		chaser.SetSpeed(enemyData.runSpeed * speedRate);
		chaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());
		chaser.SetTargetTransform(objMng.GetSpecificObject(OBJECT_KEY_PLAYER)->GetComponent<Transform>());

		auto& retChaser = stateMachine.GetState(MannequinEnemy::STATE::RETURN).GetComponent<AStarChaser>();
		retChaser.SetSpeed(enemyData.walkSpeed * speedRate);
		retChaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());
	}

	// ゾンビ
	for (int i = 0; i < zombieNum; i++)
	{
		if (static_cast<int>(zombieData_.size()) <= i)break;

		const auto& enemyData = zombieData_.at(i);


		object = make_shared<ZombieEnemy>(OBJECT_KEY_ENEMY_ZOMBIE + to_string(i));
		object->Init();
		objMng.AddObject(object);

		object->SetTransform(enemyData.spawnTransform);


		auto& animationCon = object->GetComponent<AnimationController>();
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_WALK);
		animationCon.SetAnimPlaySpeed(speedRate, ANIMATION_KEY_ENEMY_RUN);

		auto& stateMachine = object->GetComponent<StateMachine<ZombieEnemy::STATE>>();
		auto& chaser = stateMachine.GetState(ZombieEnemy::STATE::CHASE).GetComponent<AStarChaser>();

		chaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());
		chaser.SetTargetTransform(objMng.GetSpecificObject(OBJECT_KEY_PLAYER)->GetComponent<Transform>());

		chaser.SetSpeed(enemyData.runSpeed* speedRate);

		auto& retChaser = stateMachine.GetState(ZombieEnemy::STATE::RETURN).GetComponent<AStarChaser>();
		retChaser.SetSpeed(enemyData.walkSpeed* speedRate);
		retChaser.SetNavMesh(
			static_pointer_cast<Map>(objMng.GetSpecificObject(OBJECT_KEY_MAP))->GetNevMesh());

		auto& walker = stateMachine.GetState(ZombieEnemy::STATE::DEFAULT).GetComponent<RouteWalker>();
		walker.SetSpeed(enemyData.walkSpeed* speedRate);
	}
}

void ObjectSpawner::LoadJsonData()
{
	LoadLevelJsonData();

	LoadEnemyJsonData("AlienData", alienData_);
	LoadEnemyJsonData("GirlData", girlData_);
	LoadEnemyJsonData("MannequinData", mannequinData_);
	LoadEnemyJsonData("SlenderManData", slenderManData_);
	LoadEnemyJsonData("ZombieData", zombieData_);
}

void ObjectSpawner::LoadLevelJsonData()
{
	using json = nlohmann::json;
	string path = "Resource/Data/Json/";
	// JSONファイルからデータ(パス)読み込み
	// 外部ファイルの読み込み
	ifstream ifs(path + "GameDifficulty.json");
	// jsonオブジェクト
	json gameDifficultyData;
	if (ifs)
	{
		// ファイルストリームからjsonオブジェクトに変換(パース)
		ifs >> gameDifficultyData;
	}
	else
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	const auto& datas = gameDifficultyData["GameDifficulty"];

	for (auto& data : datas) {
		int level = data["Level"];

		float speedRate = data["SpeedRate"];
		float searchRate = data["SearchRate"];
		int alienNum = data["AlienNum"];
		int girlNum = data["GirlNum"];
		int mannequinNum = data["MannequinNum"];
		int slenderManNum = data["SlenderManNum"];
		int zombieNum = data["ZombieNum"];

		LEVEL_DATA levelData = {};
		levelData.speedRate = speedRate;
		levelData.searchRate = searchRate;
		levelData.alienNum = alienNum;
		levelData.girlNum = girlNum;
		levelData.mannequinNum = mannequinNum;
		levelData.slenderManNum = slenderManNum;
		levelData.zombieNum = zombieNum;

		levelData_[level] = levelData;
	}
}

void ObjectSpawner::LoadEnemyJsonData(const string& enemyName, map<int, ENEMY_DATA>& enemyDataArray)
{
	using json = nlohmann::json;
	string path = "Resource/Data/Json/";
	// JSONファイルからデータ(パス)読み込み
	// 外部ファイルの読み込み
	ifstream ifs(path + enemyName + ".json");
	// jsonオブジェクト
	json enemyData;
	if (ifs)
	{
		// ファイルストリームからjsonオブジェクトに変換(パース)
		ifs >> enemyData;
	}
	else
	{
		// 外部ファイルの読み込み失敗
		return;
	}

	const auto& dataArray = enemyData[enemyName.c_str()];

	for (auto& data : dataArray) {
		int id = data["ID"];

		float walkSpeed = data["WalkSpeed"];
		float runSpeed = data["RunSpeed"];

		Position3D spawnPos = { data["Pos"][0],data["Pos"][1], data["Pos"][2] };
		Quaternion spawnQua = Quaternion::Euler(
			{
				Deg2Radian(static_cast<float>(data["Rotate"][0])),
				Deg2Radian(static_cast<float>(data["Rotate"][1])),
				Deg2Radian(static_cast<float>(data["Rotate"][2])),
			}
			);
		Scale3D spawnScale = { data["Scale"],data["Scale"], data["Scale"] };

		ENEMY_DATA levelData = {};
		levelData.walkSpeed = walkSpeed;
		levelData.runSpeed = runSpeed;
		levelData.spawnTransform.pos = spawnPos;
		levelData.spawnTransform.qua = spawnQua;
		levelData.spawnTransform.scale = spawnScale;

		enemyDataArray[id] = levelData;
	}
}
