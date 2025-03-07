#pragma once
#include "../Component.h"
#include "../../../Utility/Math/Quaternion.h"

/// @brief トランスフォーム(位置,回転,大きさ)
class Transform final : public Component
{
public:

	[[nodiscard]] Transform();
	[[nodiscard]] Transform(Position3D initPos, Quaternion initQua, Scale3D initScale = { 1.0f,1.0f,1.0f });
	~Transform() = default;

	Position3D pos;			/// @brief 座標
	Quaternion qua;			/// @brief 回転
	Scale3D scale;			/// @brief 大きさ

	Position3D localPos;	/// @brief ローカル座標
	Quaternion localQua;	/// @brief ローカル回転
	Scale3D localScale;		/// @brief ローカル大きさ

	Position3D prePos;		/// @brief 1フレーム前座標
	Quaternion preQua;		/// @brief 1フレーム前回転
	Scale3D preScale;		/// @brief 1フレーム前大きさ

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::TRANSFORM; }

	/// @brief 更新処理
	void UpdateComponent()override {};

	/// @brief 通常Update前に走る処理
	void PreUpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override {};
};

