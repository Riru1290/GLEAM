#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Scene/SceneID.h"

#define UIMng UIManager::GetInstance()

class UIObject;

/// @brief UI管理
class UIManager final : public StaticSingleton<UIManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 実行
	void Execute();

	/// @brief UI追加
	/// @param key 任意の識別キー
	/// @param UI UIオブジェクト
	void AddUI(const string& key, shared_ptr<UIObject> UI);

	/// @brief UI削除
	/// @param key 任意の識別キー
	void DeleteUI(const string& key);

	/// @brief 全UI削除
	void ClearUIs();

	/// @brief 指定UI取得
	/// @param key 識別キー
	/// @return UI
	[[nodiscard]] weak_ptr<UIObject> GetUI(const string& key);

private:

	UIManager();
	~UIManager();
	PROOF_OF_STATIC_SINGLETON(UIManager);

	unordered_map<string, shared_ptr<UIObject>> UIObjects_;
};

