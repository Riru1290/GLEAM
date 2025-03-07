#pragma once
#include "../Object.h"

class SmoothRotateController;
class BoxCollider;

/// @brief ゴールを阻むドア
class Door final : public Object
{
public:

	[[nodiscard]] Door(const string& uniqueKey, int padLockNum);
	~Door() = default;

	/// @brief ドアに鍵をかける
	/// @param padLockNum 南京錠の数
	void LockDoor(int padLockNum) { padLockNum_ = padLockNum; }

	/// @brief 南京錠を解除する
	void UnlockPadlockOnDoor();

	/// @brief ドアを開ける
	void OpenDoor();

	/// @brief ドアを閉める
	void CloseDoor();

private:

	enum class STATE
	{
		OPEN,				/// @brief 開いている
		CLOSE,				/// @brief 閉まっている
		ROTATE_TO_OPEN,		/// @brief 回転中
		ROTATE_TO_CLOSE,	/// @brief 回転中
	};

	/// @brief オブジェクトID取得
	/// @return オブジェクトID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::DOOR; }

	/// @brief 初期化処理
	void InitObject() override;

	/// @brief 描画処理
	void DrawObject() override {}

	/// @brief 基本的な更新処理
	void UpdateObject() override;

	/// @brief コライダー設定
	void ColliderSetting();

	optional<reference_wrapper<SmoothRotateController>> rotateCon_;
	optional<reference_wrapper<BoxCollider>> boxCollider_;

	int padLockNum_;	/// @brief 南京錠の数

	STATE state_;
};

