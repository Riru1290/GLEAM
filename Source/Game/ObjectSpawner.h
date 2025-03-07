#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Object/Component/Common/Transform.h"

#define GameObjectSpawner ObjectSpawner::GetInstance()

/// @brief �Q�[����Փx�����ɃI�u�W�F�N�g�𐶐�
class ObjectSpawner final : public StaticSingleton<ObjectSpawner>
{
public:

	/// @brief �������
	void Terminate() override;


	/// @brief �I�u�W�F�N�g�𐶐�
	/// @param level ��Փx
	void ObjectSpawn(int level);

private:

	// ��Փx���Ƃ̐ݒ�
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

	// �G�l�~�[���
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

