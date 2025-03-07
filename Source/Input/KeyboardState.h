#pragma once

/// @brief キーボード入力管理
class KeyboardState
{
public:

	[[nodiscard]] KeyboardState(const HWND& hWnd);
	~KeyboardState();

	/// @brief 更新処理
	void Update();

	/// @brief 押下判定
	/// @param key キー
	/// @param buffer バッファ番号
	/// @return  押下されたか
	[[nodiscard]] bool Pressed(unsigned int key , unsigned int buffer = defaultBuffer_);

	/// @brief キーが押された瞬間の判定
	/// @param key キー
	/// @return 押された瞬間か
	[[nodiscard]] bool Triggered(unsigned int key);

	/// @brief 離された瞬間の判定
	/// @param key キー
	/// @return 離された瞬間か
	[[nodiscard]] bool Released(unsigned int key);

	/// @brief 何かしらキーが押された
	/// @return 何かしらキーが押されたか
	[[nodiscard]] bool AnyPressed();

	/// @brief 何かしらキーが離された
	/// @return 何かしらキーが離された
	[[nodiscard]] bool AnyTriggered();



private:

	unsigned int currentBuffer_;						/// @brief 現在のバッファ番号
	static constexpr size_t defaultBuffer_ = UINT_MAX;	/// @brief デフォルトのバッファ番号

	BYTE state_[2][256];		/// @brief キーボードの入力情報


};

