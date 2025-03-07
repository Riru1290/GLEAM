#pragma once
#include "../Collider.h"
#include "../../../../Collision/ColliderInfo.h"

/// @brief 球体コライダー
class SphereCollider final : public Collider
{
public:

	[[nodiscard]] SphereCollider() = default;
	~SphereCollider() = default;

	/// @brief 球体情報をセット
	/// @param info サークル情報
	void SetSphereInfo(SPHERE_COL_INFO info)
	{
		sphere_ = info;
		sphere_.Init();
		PreUpdateCollider();
	}

	void SetPosition(Position3D pos);

	[[nodiscard]] SPHERE_COL_INFO& GetSphereInfo() { return sphere_; }
	[[nodiscard]] SPHERE_COL_INFO& GetPreSphereInfo() { return preSphere_; }

	/// @brief コライダーの更新を追従対象に適用
	void ApplyColliderUpdateToFollower() override;

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPHERE_COLLIDER; }

	/// @brief コライダーの形を取得
	/// @return コライダーの形
	[[nodiscard]] COLLISION_SHAPE_TYPE GetShapeType()const override { return COLLISION_SHAPE_TYPE::SPHERE; }

	/// @brief 更新処理
	void UpdateCollider() override;

	/// @brief 通常Update前に走る処理
	void PreUpdateCollider() override;

	/// @brief 描画処理
	void DrawCollider() override;

	SPHERE_COL_INFO sphere_;			/// @brief 球体情報
	SPHERE_COL_INFO preSphere_;			/// @brief 1フレーム前球体情報

};

