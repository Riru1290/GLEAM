#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "PadID.h"

#define InputMng InputManager::GetInstance()

class KeyboardState;
class PadState;

/// @brief 入力タイプ
enum class INPUT_TYPE
{
	Pad,
	Keyboard
};

/// @brief 入力データ
struct INPUT_DATA
{
	INPUT_TYPE type_;
	unsigned int key_;
};

/// @brief プレイヤーID
enum class PLAYER_ID 
{
	P1,
	P2,
	P3,
	P4,
	MAX
};

/// @brief 入力情報管理
class InputManager final : public StaticSingleton<InputManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 更新処理
	void Update();

	/// @brief コンフィグ追加
	/// @param key キー
	/// @param list キーに対応させる入力データ群
	void AddKey(const string& key, vector<INPUT_DATA> list);

	/// @brief 押下判定
	/// @param key キー
	/// @param padNo パッド番号(指定しなければキーボード判定)
	/// @return  押下されたか
	[[nodiscard]] bool Pressed(const string& key,UINT padNo = UINT_MAX);

	/// @brief キーが押された瞬間の判定
	/// @param key キー
	/// @param padNo パッド番号(指定しなければキーボード判定)
	/// @return 押された瞬間か
	[[nodiscard]] bool Triggered(const string& key, UINT padNo = UINT_MAX);

	/// @brief 離された瞬間の判定
	/// @param key キー
	/// @param padNo パッド番号(指定しなければキーボード判定)
	/// @return 離された瞬間か
	[[nodiscard]] bool Released(const string& key, UINT padNo = UINT_MAX);

	/// @brief 長押し判定
	/// @param key キー
	/// @param padNo パッド番号(指定しなければキーボード判定)
	/// @return  指定時間長押ししていたか
	[[nodiscard]] bool Hold(float time, const string& key, UINT padNo = UINT_MAX);

	/// @brief 左トリガーの押し込み量取得
	/// @param padNo パッド番号 
	/// @return 左トリガーの押し込み量0(min)~255(max)
	[[nodiscard]] int GetLeftTriggerState(UINT padNo = PAD_1);

	/// @brief 右トリガーの押し込み量取得
	/// @param padNo パッド番号 
	/// @return 右トリガーの押し込み量0(min)~255(max)
	[[nodiscard]] int GetRightTriggerState(UINT padNo = PAD_1);


	/// @brief 左スティックの移動量量取得
	/// @param padNo パッド番号 
	/// @return 左スティックの移動量  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetLeftStickState(UINT padNo = PAD_1);

	/// @brief 右スティックの移動量量取得
	/// @param padNo パッド番号 
	/// @return 右スティックの移動量  -32768(min)~32767(max)
	[[nodiscard]] Vector2 GetRightStickState(UINT padNo = PAD_1);

	/// @brief 何かしらキー(ボタン)が押された
	/// @return 何かしらキー(ボタン)が押されたか
	[[nodiscard]] bool AnyPressed(UINT padNo = UINT_MAX);

	/// @brief 何かしらキー(ボタン)が離された
	/// @return 何かしらキー(ボタン)が離された
	[[nodiscard]] bool AnyTriggered(UINT padNo = UINT_MAX);

	/// @brief 現在接続されているパッドの数取得
	/// @return 現在接続されているパッドの数
	[[nodiscard]] const int& GetConnectingPadNum()const { return connectingPadNum_; };


	/// @brief 指定されたキーに対応するコンフィグ設定を返す
	/// @param key キー
	/// @return 指定されたキーに対応するコンフィグ設定
	[[nodiscard]] vector<INPUT_DATA> GetKeyConfig(const string& key);

	/// @brief マウス移動量取得
	/// @return マウス移動量
	[[nodiscard]] const Vector2& GetMouseMovePow() { return mouseMovePow_; }

	/// @brief マウス座標取得
	/// @return マウス座標
	[[nodiscard]] const Position2D& GetMousePosition() { return mousePos_; }

	/// @brief 1フレーム前マウス座標取得
	/// @return 1フレーム前マウス座標
	[[nodiscard]] const Position2D& GetPreMousePosition() { return preMousePos_; }

	/// @brief Windowからマウスの移動量を取得
	/// @param mouseMovePow マウスの移動量
	void SetMouseMovePowFromWindow(Vector2 mouseMovePow);

	/// @brief マウス座標を固定化するかどうか
	/// @param isFixedMouse マウス座標を固定化するかどうか
	void SetIsFixedMouse(bool isFixedMouse) { isFixedMouse_ = isFixedMouse; }

private:

	InputManager();
	~InputManager();
	PROOF_OF_STATIC_SINGLETON(InputManager);

	/// @brief マウス情報更新
	void MouseDataUpdate();

	unique_ptr<KeyboardState> keyboard_;		/// @brief キーボード

	static constexpr int PAD_NUM_MAX = (4);		/// @brief パッド最大数
	unique_ptr<PadState> pad_[PAD_NUM_MAX];		/// @brief パッド

	unsigned int connectingPadNum_ = 0;			/// @brief 現在接続されているパッドの数

	Position2D mouseMovePow_ = {};				/// @brief マウス移動量
	Position2D mousePos_ = {};					/// @brief マウス座標
	Position2D preMousePos_ = {};				/// @brief 1フレーム前マウス座標

	bool isFixedMouse_ = false;					/// @brief カーソルを固定するか

	unordered_map<string, vector<INPUT_DATA>> keyConfig_;	/// @brief キーコンフィグリスト

	unordered_map<string, float> keyHoldTime_;				/// @brief キーを連続して押していた時間

};

