#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Resource/Model/ModelData.h"

/// @brief MIZファイルローダー
class MIZLoader final : public StaticSingleton<MIZLoader>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief MIZファイル読み込み
	/// @param filePath ファイルパス
	/// @return モデルデータ
	[[nodiscard]] MODEL_DATA LoadModel(const string& filePath);

	/// @brief MIZファイル読み込み
	/// @param filePath ファイルパス
	/// @return アニメーションデータ
	[[nodiscard]] ANIMATION_DATA LoadAnimation(const string& filePath);

private:

	MIZLoader();
	~MIZLoader();
	PROOF_OF_STATIC_SINGLETON(MIZLoader);
};

