#pragma once
#include "../Component.h"

class InputManager;

/// @brief 入力方法
enum class INPUT_PUSH_TYPE
{
	PRESS,
	TRRIGER,
	RELEASE
};

/// @brief 入力コントローラ
class InputController final : public Component
{
public:

	[[nodiscard]] InputController();
	~InputController() = default;

	/// @brief キーと入力方法に応じた動作を追加
	/// @param key キー
	/// @param pushType 入力方法
	/// @param func 動作
	void AddFunction(const string& key, INPUT_PUSH_TYPE pushType, function<void()> func);

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::INPUT_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent()override;

	optional<reference_wrapper<InputManager>> inputMng_;		/// @brief InputManager参照

	unordered_multimap<string, pair<INPUT_PUSH_TYPE, function<void()>>> controllList_;	/// @brief コントロールリスト

	unordered_map<INPUT_PUSH_TYPE, function<bool(const string&)>> isAction_;			/// @brief 入力キーに応じた関数呼び出し

};

