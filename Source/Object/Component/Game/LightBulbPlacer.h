#pragma once
#include "../Component.h"

class LightBulb;
class Transform;
class ModelRenderer;

/// @brief ライトを設置する
class LightBulbPlacer final : public Component
{
public:

	[[nodiscard]] LightBulbPlacer();
	~LightBulbPlacer();

	/// @brief 事前に設定された場所にライトを設置
	void PlaceLightBulb();

	/// @brief ライトを設置
	/// @param transform 設置情報
	void PlaceLightBulb(Transform transform);

	/// @brief 設置可能なライトの上限値を設定
	/// @param maxNum 
	void SetLightBulbNumMax(int maxNum);

	/// @brief ライト設置情報を設定
	/// @param transform ライト設置情報
	void SetPlaceLightTransform(Transform transform) { placeLightTransform_ = transform; }

	/// @brief 事前に設定された指定のライトを回収
	void GetLightBulb();

	/// @brief ライト回収情報を設定
	/// @param key ライト回収情報
	void SetGetLightBulbObjectUniqueKey(string key) { getLightKey_ = key; }

	void SetIsPossibleCheckFunction(function<bool()> func);

	/// @brief ライト保持数を取得
	/// @return ライト保持数
	[[nodiscard]] const int& GetLightBulbNum()const { return lightBulbNum_; }

	void SetTempLightBulbModelTransState(bool isTrans);

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	virtual COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::LIGHTBULB_PLACER; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 解放処理
	void ReleaseComponent() override {}


	int lightBulbNum_;							/// @brief 現在保持中のライトの数
	int placeLightBulbNum_;						/// @brief 現在設置中のライトの数
	int lightBulbNumMax_;						/// @brief 設置可能なライトの数
	unordered_map<string,bool> lightBulbKey_;	/// @brief ライト管理キー&設置状態か否か

	Transform placeLightTransform_;

	string getLightKey_;

	function<bool()> IsPossible_;	/// @brief ライト設置状態かどうか

	optional<reference_wrapper<ModelRenderer>> temporaryLightBulbModel_;
	weak_ptr<LightBulb> temporaryLightBulb_;
};

