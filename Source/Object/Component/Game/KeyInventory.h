#pragma once
#include "../Component.h"
#include "../../../Game/Event/EventID.h"


/// @brief 鍵を管理する
class KeyInventory final : public Component
{
public:

	[[nodiscard]] KeyInventory();
	~KeyInventory() = default;

	/// @brief 鍵を使用する
	/// @return 使用できたかどうか
	[[nodiscard]] bool UseKey();

	/// @brief 事前に設定された鍵を取得
	void GetKey();

	/// @brief 指定の鍵を取得
	/// @param keyName 鍵のキー
	void GetKey(const string& keyName);

	/// @brief 取得する鍵のオブジェクト識別キーを設定
	/// @param uniqueKey 取得する鍵のオブジェクト識別キー
	void SetGetKeyObjectUniqueKey(const string& uniqueKey) { getKeyObjectUniqueKey_ = uniqueKey; }

	/// @brief 現在取得している鍵の数を取得
	/// @return 現在取得している鍵の数
	[[nodiscard]] const int& GetKeyCount()const { return getKeyCount_; }


private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	virtual COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::KEY_INVENTORY; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 解放処理
	void ReleaseComponent() override {}


	int getKeyCount_;				/// @brief 鍵取得数

	string getKeyObjectUniqueKey_;	/// @brief 取得する鍵のオブジェクト識別キー

	set<EVENT_ID> useIventID_;
	//vector<string> getKeyName_;
};

