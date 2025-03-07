#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Scene/SceneID.h"
#include "ResourceID.h"
#include "Resource.h"

#define ResourceMng ResourceManager::GetInstance()

class Texture;
class Model;
class Animation;
class Sound;
class EffekseerEffect;

/// @brief リソース管理
class ResourceManager final : public StaticSingleton<ResourceManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 指定のシーンに対応したリソースを読み込み&その他リソースの解放
	/// @param sceneID 
	void LoadAndRelease(SCENE_ID sceneID);

	/// @brief テクスチャの追加読み込み
	/// @param path テクスチャパス
	/// @return テクスチャデータ
	const Texture& LoadAdditionalTexture(const string& path);

	/// @brief テクスチャをリソースマネージャーに保存
	/// @param texture テクスチャ
	/// @return テクスチャデータ
	const Texture& RegisterTextureResource(const string& key, unique_ptr<Texture> texture);

	/// @brief テクスチャデータ取得
	/// @param id リソースID
	/// @return テクスチャ
	const Texture& GetTexture(RES_ID id);

	/// @brief テクスチャデータ取得
	/// @param key キー
	/// @return テクスチャ
	const Texture& GetTexture(const string& key);

	/// @brief モデルデータ取得
	/// @param id リソースID
	/// @return モデル
	const Model& GetModel(RES_ID id);

	/// @brief アニメーションデータ取得
	/// @param id リソースID
	/// @return アニメーション
	const Animation& GetAnimation(RES_ID id);

	/// @brief サウンドデータ取得
	/// @param id リソースID
	/// @return サウンド
	const Sound& GetSound(RES_ID id);

	/// @brief エフェクトデータ取得
	/// @param id リソースID
	/// @return エフェクト
	const EffekseerEffect& GetEffect(RES_ID id);

private:

	/// @brief リソース種別
	enum class RES_TYPE
	{
		IMAGE,
		MODEL,
		ANIMATION,
		SOUND,
		EFFECT,
		MAX
	};

	/// @brief リソース構成データ
	struct RES_DATA
	{
		RES_TYPE resType;
		RES_ID resID;
		string path;
		string ext;
	};

	ResourceManager();
	~ResourceManager();
	PROOF_OF_STATIC_SINGLETON(ResourceManager);

	/// @brief リソースパスをJSONファイルから読み込み
	void LoadJsonData();

	/// @brief リソースを読み込み
	/// @param data リソース構成データ
	void Load(RES_DATA data);

	/// @brief リソースを追加読み込み
	/// @param data リソース構成データ
	/// @param key 追加リソース識別キー
	void AdditionalLoad(RES_DATA data, const string& key);

	/// @brief 指定リソースを解放
	/// @param resID リソースID
	void Release(RES_ID resID);

	/// @brief 指定追加リソースを解放
	/// @param key 追加リソース識別キー
	void Release(const string& key);

	unordered_map<SCENE_ID, vector<RES_DATA>> pathData_;		/// @brief ファイルパスデータ

	unordered_map<RES_ID, unique_ptr<Resource>> loadedData_;	/// @brief ロード済みデータ
	unordered_map<string, unique_ptr<Resource>> addLoadedData_;	/// @brief 追加ロード済みデータ

	mutex resourceGuard_;										/// @brief リソース分割読み込み用mutex
};

