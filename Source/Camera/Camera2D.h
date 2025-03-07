#pragma once
#include "../Common/Singleton/StaticSigleton.h"

#define MainCamera2D Camera2D::GetInstance()

/// @brief カメラ2D
class Camera2D final : public StaticSingleton<Camera2D>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief カメラ状態
	enum class STATE
	{
		FREE = 0,	// フリーカメラ
		FIXED,		// 固定
	};


	/// @brief カメラ状態切り替え
	/// @param state 切り替え状態
	void ChangeState(STATE state);

	/// @brief 更新処理
	void Update();

	/// @brief カメラ情報をシェーダにセット
	void SetBeforeDraw();

private:

	struct Camera2DInfo;

	Camera2D();
	~Camera2D();
	PROOF_OF_STATIC_SINGLETON(Camera2D);

	/// @brief 更新処理:フリー
	void UpdateFree();

	/// @brief 更新処理:固定
	void UpdateFixed();

	/// @brief ビュープロジェクション行列バッファ作成
	void CreateConstantBufferScreenMatrix();

	/// @brief カメラ情報定数バッファ更新
	void UpdateScreenMatrix();



	STATE state_;			/// @brief 現在の状態

	Position2D pos_;		/// @brief 座標
	float angle_;			/// @brief 角度	
	float lens_;			/// @brief カメラ倍率

	bool isChanging_;		/// @brief カメラ情報に変更があったか


	function<void()> StateUpdate_;								/// @brief 状態別更新処理 
	unordered_map<STATE, function<void()>> stateUpdateList_;	/// @brief 状態別更新処理リスト

	ComPtr<ID3D12Resource> cameraConstBuffer_;				/// @brief カメラ情報バッファ
	int cameraHeapNo_;										/// @brief カメラ情報ディスクリプタヒープ番号
	Camera2DInfo* cameraMap_;								/// @brief バッファ更新用変数

};

/// @brief カメラ情報
struct Camera2D::Camera2DInfo
{
	Position2D pos = {};	/// @brief 座標
	float angle = 0.0f;		/// @brief 角度
	float lens = 1.0f;		/// @brief 拡大率
	XMMATRIX screenMat = {};/// @brief スクリーン行列
};

