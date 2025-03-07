#pragma once
#include "../Component.h"

/// @brief 南京錠を解除する
class PadLockUnlocker final : public Component
{
public:

	[[nodiscard]] PadLockUnlocker();
	~PadLockUnlocker() = default;

	/// @brief 事前に設定された南京錠を解除
	void UnLock() const;

	/// @brief 解除する南京錠のオブジェクト識別キーを設定
	/// @param uniqueKey 解除する南京錠のオブジェクト識別キー
	void SetGetKeyObjectUniqueKey(const string& uniqueKey) { padLockObjectUniqueKey_ = uniqueKey; }

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	virtual COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::PADLOCK_UNLOCKER; }

	/// @brief 更新処理
	void UpdateComponent() override {}

	/// @brief 解放処理
	void ReleaseComponent() override {}


	string padLockObjectUniqueKey_;	/// @brief 解除する南京錠のオブジェクト識別キー

};

