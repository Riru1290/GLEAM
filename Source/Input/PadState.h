#pragma once
#include "PadID.h"

/// @brief パッド(XInput)入力管理
class PadState
{
public:

	[[nodiscard]] PadState(UINT padNo = PAD_1);
	~PadState();

	/// @brief 更新処理
	/// @return 接続されていなければfalseを返す
	[[nodiscard]] bool Update();

	/// @brief 押下判定
	/// @param button ボタン
	/// @param buffer バッファ番号
	/// @return  押下されたか
	[[nodiscard]] bool Pressed(unsigned int button, unsigned int buffer = defaultBuffer_);

	/// @brief キーが押された瞬間の判定
	/// @param button ボタン
	/// @return 押された瞬間か
	[[nodiscard]] bool Triggered(unsigned int button);

	/// @brief 離された瞬間の判定
	/// @param button ボタン
	/// @return 離された瞬間か
	[[nodiscard]] bool Released(unsigned int button);

	/// @brief 左トリガーの押し込み量取得
	/// @param buffer バッファ番号
	/// @return 左トリガーの押し込み量0(min)~255(max)
	[[nodiscard]] int GetLeftTriggerState(unsigned int buffer = defaultBuffer_);

	/// @brief 右トリガーの押し込み量取得
	/// @param buffer バッファ番号
	/// @return 右トリガーの押し込み量0(min)~255(max)
	[[nodiscard]] int GetRightTriggerState(unsigned int buffer = defaultBuffer_);

	/// @brief 左スティックの移動量量取得
	/// @param buffer バッファ番号 
	/// @return 左スティックの移動量  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetLeftStickState(unsigned int buffer = defaultBuffer_);

	/// @brief 右スティックの移動量量取得
	/// @param buffer バッファ番号 
	/// @return 右スティックの移動量  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetRightStickState(unsigned int buffer = defaultBuffer_);
		
	/// @brief 何かしらボタンが押された
	/// @return 何かしらボタンが押されたか
	[[nodiscard]] bool AnyPressed();

	/// @brief 何かしらボタンが離された
	/// @return 何かしらボタンが離された
	[[nodiscard]] bool AnyTriggered();

private:

	unsigned int currentBuffer_;						/// @brief 現在のバッファ番号
	static constexpr size_t defaultBuffer_ = UINT_MAX;	/// @brief デフォルトのバッファ番号

	XINPUT_STATE state_[2];		/// @brief キーボードの入力情報

	UINT padNo_;				/// @brief パッド番号
};

