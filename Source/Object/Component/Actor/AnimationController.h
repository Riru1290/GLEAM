#pragma once
#include "../Component.h"
#include "../../../Resource/Model/ModelData.h"

class AnimationModelRenderer;
struct ANIM_DATA;

/// @brief モデルアニメーションコントローラ
class AnimationController final : public Component
{
public:

	[[nodiscard]] AnimationController();
	~AnimationController() = default;

	/// @brief アニメーションさせるモデルをセット
	/// @param modelRenderer モデルレンダラー
	void SetAnimationModel(AnimationModelRenderer& modelRenderer);

	/// @brief アニメーション追加
	/// @param animData アニメーションデータ
	/// @param animKey アニメーションキー
	void AddAnimation(ANIMATION_DATA& animData,const string& animKey);

	/// @brief アニメーション変更
	/// @param animKey アニメーションキー
	void ChangeAnimation(const string& animKey);

	/// @brief アニメーション終了判定
	/// @param animKey アニメーションキー
	/// @return アニメーションが終了したか
	[[nodiscard]] bool IsEndAnimation(const string& animKey) const;
	
	/// @brief ループ設定
	/// @param animKey アニメーションキー
	/// @param isLoop ループさせるか
	void SetIsLoop(bool isLoop, const string& animKey);

	/// @brief ストップ設定
	/// @param animKey アニメーションキー
	/// @param isStop ストップさせるか
	void SetIsStop(bool isStop, const string& animKey);

	void SetAnimPlaySpeed(float speed, const string& animKey);
	void SetAnimPlaySpeed(float speed);

	/// @brief アニメーション終了時に呼ばれる関数をセット
	/// @param animKey アニメーションキー
	/// @param isLoop アニメーション終了時に呼ばれる関数
	void SetOnAnimEndFunction(function<void()> OnAnimEnd, const string& animKey);


private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ANIMATION_CONTROLLER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 描画処理
	void DrawComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override;

	/// @brief アニメーション時間を更新
	void UpdateAnimationTime();

	/// @brief アニメーションブレンド率を更新
	void UpdateAnimationBlendRate();

	/// @brief アニメーション行列を更新
	void UpdateAnimationMatrix();

	/// @brief フレーム補間後のアニメーション行列を返す
	/// @param data 行列配列
	/// @param frame フレーム
	/// @return フレーム補間後のアニメーション行列
	[[nodiscard]] XMMATRIX GetFrameInterpolatedAnimationMatrix(const vector<XMMATRIX>& data, float frame, bool isLoop);

	string currentKey_;									/// @brief 現在優先されているアニメーションキー
	unordered_map<string, ANIM_DATA> animations_;		/// @brief アニメーションデータ
	bool isChangingAnimation_;							/// @brief アニメーション遷移中か

	optional<reference_wrapper<AnimationModelRenderer>> modelRenderer_;	/// @brief アニメーション適用先モデル
};

struct ANIM_DATA
{
	// 変わらないもの------------------------------

	string animName = "";							/// @brief アニメーション名

	map<string, vector<XMMATRIX>> animBoneMatrix;	/// @brief ボーンごと時間ごとのアニメーション行列

	float start = 0.0f;				/// @brief アニメーション開始時間(初期値)
	float end = 0.0f;				/// @brief アニメーション終了時間(初期値)
	float total = 0.0f;				/// @brief アニメーション総再生時間(初期値)

	float frameRate = 60.0f;		/// @brief アニメーションフレームレート

	// 変わる可能性のあるもの----------------------

	bool isAnimation = false;		/// @brief アニメーションが現在適用されているかどうか

	float animTime = 0.0f;			/// @brief アニメーション現在時間
	float animStartTime = 0.0f;		/// @brief アニメーション開始時間
	float animEndTime = 0.0f;		/// @brief アニメーション終了時間
	float animTotalTime = 0.0f;		/// @brief アニメーション総再生時間

	float animSpeed = 1.0f;			/// @brief アニメーションスピード

	bool isLoop = true;				/// @brief ループさせるか
	bool isStop = false;			/// @brief 再生をストップさせるか

	float animBlendRate = 0.0f;		/// @brief ブレンド率
	float animAttachSpeed = 1.5f;	/// @brief ブレンド率増加速度
	float animDetachSpeed = 1.5f;	/// @brief ブレンド率減少速度

	function<void()> OnAnimEnd;		/// @brief アニメーション終了時に呼ぶ関数
};
