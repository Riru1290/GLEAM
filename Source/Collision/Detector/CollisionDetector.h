#pragma once
#include "../CollisionHitData.h"

class CollisionDetectFunction;

#define ColDetector CollisionDetector::GetInstance()

/// @brief 衝突判定を行う
class CollisionDetector final : public StaticSingleton<CollisionDetector>
{
public:

	/// @brief 解放処理
	void Terminate() override {};

	/// @brief 衝突判定
	/// @param index 2つのコライダーの形の組み合わせによって変わるインデックス
	/// @param colA コライダーA
	/// @param colB コライダーB
	/// @param hitData 衝突時データ
	/// @return 衝突しているかどうか
	[[nodiscard]] bool Detection(int index,
		weak_ptr<Collider> colA, weak_ptr<Collider> colB, COL_HIT_DATA& hitData);

protected:

	CollisionDetector();
	~CollisionDetector();
	PROOF_OF_STATIC_SINGLETON(CollisionDetector);
	
	map<int, unique_ptr<CollisionDetectFunction>> detectFunctions_;		/// @brief 衝突判定

};

