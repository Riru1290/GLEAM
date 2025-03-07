#pragma once
#include "../Object.h"
#include "../../Algorithm/NavMesh.h"

class ModelRenderer;

class Map final : public Object
{
public:

	[[nodiscard]] Map(const string& uniqueKey);
	~Map();

	/// @brief ナビメッシュ取得
	/// @return ナビメッシュ
	[[nodiscard]] const NavMesh& GetNevMesh()const { return navMesh_; }

private:

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::MAP; }

	/// @brief 描画処理
	void DrawObject() override;

	/// @brief 基本的な更新処理
	void UpdateObject() override;

	/// @brief 通常Update後に走る処理
	void LateUpdateObject() override;

	/// @brief カメラ更新後に走る処理
	void OnCameraUpdateObject() override;

	/// @brief コライダー設定
	void ColliderSetting();

	shared_ptr<ModelRenderer> renderer;

	NavMesh navMesh_;
};


