#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Utility/Math/Quaternion.h"
#include "../Common/AABB/Frustum.h"
#include "../Common/AABB/BoundingSphere.h"
#include "../Object/Component/Common/Transform.h"

#define MainCamera Camera::GetInstance()

/// @brief カメラ
class Camera final : public StaticSingleton<Camera>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief カメラ状態
	enum class STATE
	{
		FREE = 0,	// フリーカメラ
		FOLLOW,		// 追従
		RATE_FOLLOW,// 割合追従
		FIXED,		// 固定
		LERP,		// ラープ
		LERP_FOLLOW,// 追従ラープ
	};


	/// @brief カメラ状態切り替え
	/// @param state 切り替え状態
	/// @param isReset カメラを初期化させるか
	void ChangeState(STATE state, bool isReset = false);

	/// @brief 更新処理
	void Update();

	/// @brief カメラ情報をシェーダにセット
	void SetBeforeDraw();

	/// @brief カメラ座標セット
	/// @param pos カメラ座標
	void SetPosition(Position3D pos) { pos_ = pos; }

	/// @brief カメラ回転セット
	/// @param pos カメラ回転
	void SetRotate(Quaternion rot);

	/// @brief 追従対象セット
	/// @param follower 追従対象
	void SetFollower(const Transform& follower) { follower_ = follower; }

	/// @brief 追従時相対座標をセット
	/// @param relativePos 追従時相対座標
	void SetFollowRelativePos(Position3D relativePos) { followRelativePos_ = relativePos; }

	/// @brief 追従割合セット
	/// @param rate 追従割合(0.0f～1.0f)
	void SetFollowRate(const float& rate) { followRate_ = rate; }

	/// @brief 追従対象(サブ)セット
	/// @param follower 追従対象(サブ)
	void SetSubFollower(const Transform& follower) { subFollower_ = follower; }

	/// @brief 追従(サブ)時相対座標をセット
	/// @param relativePos 追従(サブ)時相対座標
	void SetSubFollowRelativePos(Position3D relativePos) { subFollowRelativePos_ = relativePos; }

	/// @brief ラープ時設定
	/// @param before 開始カメラ設定
	/// @param after  終了カメラ設定
	/// @param time ラープ時間
	void SetLerpConfig(const Transform& before, const Transform& after, float time);

	/// @brief ラープ時設定
	/// @param after  終了カメラ設定
	/// @param time ラープ時間
	void SetLerpConfig(const Transform& after, float time);

	/// @brief 追従ラープ時設定
	/// @param after  終了カメラ設定
	/// @param time ラープ時間
	void SetLerpFollowConfig(const Transform& after, float time);


	/// @brief 画面揺れ
	/// @param time 揺らす時間
	/// @param pow 揺れる大きさ
	/// @param speed 揺れる速さ
	/// @param isPriority 最優先させるか
	void Shake(float time, Vector3 pow, float speed, bool isPriority = false);

	/// @brief カメラビュープロジェクションフラスタムを取得
	/// @return カメラビュープロジェクションフラスタム
	[[nodiscard]] const Frustum& GetViewProjectionFrustum()const { return frustum_; }

	[[nodiscard]] const XMMATRIX& GetViewMatrix()const { return viewMatrix_; }
	[[nodiscard]] const XMMATRIX& GetProjectionMatrix()const { return projMatrix_; }

	[[nodiscard]] const Transform& GetTransform()const { return cameraTransform_; }

	/// @brief カメラ座標を取得
	/// @return カメラ座標
	[[nodiscard]] const Position3D& GetPos() const { return pos_; }

	/// @brief ターゲット座標を取得
	/// @return ターゲット座標
	[[nodiscard]] const Position3D& GetTargetPos() const { return targetPos_; }

	/// @brief アップベクトルを取得
	/// @return アップベクトル
	[[nodiscard]] const Vector3& GetUpVector() const { return up_; }

	/// @brief 回転を取得
	/// @return 回転
	[[nodiscard]] const Quaternion& GetRotation() const { return rot_; }

	/// @brief 更新後カメラ座標を取得
	/// @return 更新後カメラ座標
	[[nodiscard]] Position3D GetLatestPos() const;

	/// @brief 更新後ターゲット座標を取得
	/// @return 更新後ターゲット座標
	[[nodiscard]] Position3D GetLatestTargetPos() const;

	/// @brief ラープが終了したか
	/// @return ラープが終了したか
	[[nodiscard]] bool IsEndLerp()const;

private:

	struct CameraInfo;

	Camera();
	~Camera();
	PROOF_OF_STATIC_SINGLETON(Camera);

	/// @brief カメラ入力設定
	void InputSetting();

	/// @brief カメラ情報リセット
	void ResetCamera();

	/// @brief 更新処理:フリー
	void UpdateFree();

	/// @brief 更新処理:追従
	void UpdateFollow();

	/// @brief 更新処理:割合追従
	void UpdateRateFollow();

	/// @brief 更新処理:固定
	void UpdateFixed();

	/// @brief 更新処理:ラープ
	void UpdateLerp();

	/// @brief 更新処理:追従ラープ
	void UpdateLerpFollow();

	/// @brief 更新処理:フリー
	void LatestUpdateFree(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief 更新処理:追従
	void LatestUpdateFollow(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief 更新処理:割合追従
	void LatestUpdateRateFollow(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief 更新処理:固定
	void LatestUpdateFixed(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief 更新処理:ラープ
	void LatestUpdateLerp(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief 更新処理:追従ラープ
	void LatestUpdateLerpFollow(Position3D& cameraPos, Position3D& targetPos)const;

	/// @brief カメラ情報定数バッファ作成
	void CreateConstantBuffer();

	/// @brief ビュープロジェクション行列バッファ作成
	void CreateConstantBufferViewProjectionMatrix();

	/// @brief カメラ情報定数バッファ作成
	void CreateConstantBufferCameraInfo();
	
	/// @brief カメラ情報定数バッファ更新
	void UpdateViewProjectionMatrix();

	/// @brief ビュープロジェクションフラスタム更新
	void UpdateFrustum(const XMMATRIX& viewProj);


	/// @brief 画面揺れ
	/// @param pos 基準座標
	void ShakeCamera(Position3D& pos);


	STATE state_;					/// @brief 現在の状態

	Position3D pos_;				/// @brief 座標
	Position3D targetPos_;			/// @brief ターゲット座標
	Vector3 up_;					/// @brief 上方向
	Quaternion rot_;				/// @brief 回転

	float fov_;						/// @brief 画角
	float near_;					/// @brief 描画近
	float far_;						/// @brief 描画遠

	Transform cameraTransform_;		/// @brief カメラ情報

	bool isChanging_;				/// @brief カメラ情報に変更があったか

	Frustum frustum_;				/// @brief ビュープロジェクションフラスタム
	BoundingSphere sphere_;			/// @brief ビュープロジェクションフラスタム


	optional<reference_wrapper<const Transform>> follower_;			/// @brief 追従対象
	Position3D followRelativePos_;									/// @brief 追従時相対座標

	float followRate_;												/// @brief 追従レート
	optional<reference_wrapper<const Transform>> subFollower_;		/// @brief 追従対象(サブ)
	Position3D subFollowRelativePos_;								/// @brief 追従時相対座標(サブ)

	Transform lerpStartTransform_;									/// @brief ラープ開始時トランスフォーム
	Transform lerpEndTransform_;									/// @brief ラープ開始時トランスフォーム

	optional<reference_wrapper<const Transform>> lerpEndFollower_;	/// @brief ラープ開始時トランスフォーム

	Vector3 shakePow_;												/// @brief シェイクする力
	float shakeSpeed_;												/// @brief シェイクする速さ
	bool isPriorityShake_;											/// @brief 優先的にシェイクするかどうか

	function<void()> StateUpdate_;									/// @brief 状態別更新処理 
	unordered_map<STATE, function<void()>> stateUpdateList_;		/// @brief 状態別更新処理リスト

	function<void(Position3D&, Position3D&)> StateLatestUpdate_;							/// @brief 状態別更新処理
	unordered_map<STATE, function<void(Position3D&, Position3D&)>> stateLatestUpdateList_;	/// @brief 状態別更新処理リスト

	ComPtr<ID3D12Resource> viewProjMatConstBuffer_;	/// @brief カメラ情報バッファ
	int viewProjHeapNo_;							/// @brief カメラ情報ディスクリプタヒープ番号
	XMMATRIX* viewProjMap_;							/// @brief バッファ更新用変数

	ComPtr<ID3D12Resource> cameraConstBuffer_;		/// @brief ビュープロジェクション行列バッファ
	int cameraHeapNo_;								/// @brief ビュープロジェクション行列ディスクリプタヒープ番号
	CameraInfo* cameraMap_;							/// @brief バッファ更新用変数

	XMMATRIX viewMatrix_;		/// @brief ビュー行列
	XMMATRIX projMatrix_;		/// @brief プロジェクション行列
};

/// @brief カメラ情報
struct Camera::CameraInfo
{
	Position3D pos = {};	/// @brief カメラ座標
	float padding1 = 0.0f;	/// @brief パディング
	Vector3 dir = {};		/// @brief カメラ方向
	float padding2 = 0.0f;	/// @brief パディング
};
