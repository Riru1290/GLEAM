#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Object/Component/Common/Transform.h"

#define GameObjectSpawner ObjectSpawner::GetInstance()

/// @brief ゲーム難易度を元にオブジェクトを生成
class ObjectSpawner final : public StaticSingleton<ObjectSpawner>
{
public:

	/// @brief 解放処理
	void Terminate() override;


	/// @brief オブジェクトを生成
	/// @param level 難易度
	void ObjectSpawn(int level);

private:

	// 難易度ごとの設定
	struct LEVEL_DATA
	{
		float speedRate;
		float searchRate;
		int alienNum;
		int girlNum;
		int mannequinNum;
		int slenderManNum;
		int zombieNum;
	};

	// エネミー情報
	struct ENEMY_DATA
	{
		float walkSpeed;
		float runSpeed;
		Transform spawnTransform;
	};

	ObjectSpawner();
	~ObjectSpawner();
	PROOF_OF_STATIC_SINGLETON(ObjectSpawner);

	void EnemySpawn(int level);

	void LoadJsonData();
	void LoadLevelJsonData();

	void LoadEnemyJsonData(const string& enemyName, map<int, ENEMY_DATA>& enemyDataArray);


	map<int, LEVEL_DATA> levelData_;

	map<int, ENEMY_DATA> alienData_;
	map<int, ENEMY_DATA> girlData_;
	map<int, ENEMY_DATA> mannequinData_;
	map<int, ENEMY_DATA> slenderManData_;
	map<int, ENEMY_DATA> zombieData_;
};

