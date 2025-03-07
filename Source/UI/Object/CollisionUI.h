#pragma once
#include "../UIObject.h"
#include "../../Collision/ColliderInfo.h"

class Rect2DCollider;
struct COL_HIT_DATA;

/// @brief 衝突判定を行うUI
class CollisionUI final : public UIObject
{
public:

	[[nodiscard]] CollisionUI();
	~CollisionUI() = default;

	/// @brief 矩形情報をセット
	/// @param rect 矩形
	void SetColliderRectInfo(RECT_COL_INFO rect);

	/// @brief 有効かどうかをセット
	/// @param isActive 有効かどうか
	void SetColliderActiveState(bool isActive);

	/// @brief 衝突後処理関数追加
	/// @param objectID 対象オブジェクト
	/// @param hitData 衝突後処理
	void AddColliderOnHitFunction(OBJECT_ID objectID, function<void(COL_HIT_DATA)> onHit);

private:

	/// @brief 基本的な更新処理
	void UpdateUI() override;

	/// @brief テクスチャ設定後の初期化
	void InitAfterSetTexture() override;

	optional<reference_wrapper<Rect2DCollider>> collider_;

	function<void(COL_HIT_DATA)> onHit_;
};

