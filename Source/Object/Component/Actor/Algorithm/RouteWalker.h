#pragma once
#include "../../Component.h"

class Transform;

struct RouteNode
{
	int id;
	Position3D pos;
	vector<int> neighbors;
};

/// @brief 指定されたルートを徘徊する
class RouteWalker final : public Component
{
public:

	[[nodiscard]] RouteWalker();
	~RouteWalker() = default;

	/// @brief 開始位置を設定
	/// @param id 開始位置ID
	void InitStart(int id);
	
	/// @brief 単純なルートを座標情報をもとに作成
	/// @param route 座標配列
	void CreateStraightRoute(vector<Position3D> route);

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief 移動速度を設定
	/// @param speed 
	void SetSpeed(float speed)
	{
		speed_ = speed;
	}

	/// @brief 移動方向取得
	/// @return 移動方向
	[[nodiscard]] const Vector3& GetMoveDir()const { return moveDir_; }

	/// @brief ゴール座標取得
	/// @return ゴール座標
	[[nodiscard]] Position3D GetGoalPosition()const { return goal_.pos; }

	/// @brief ルートの中の最も近い座標取得
	/// @return 最も近い座標
	[[nodiscard]] Position3D GetNeaPosition(const Position3D pos)const;

	/// @brief 指定した座標に最も近いルート上のノードをゴールとする
	[[nodiscard]] void SetNextNodeToNearestRoutePoint(const Position3D pos);

	/// @brief 歩いた際のアクション設定
	/// @param action アクション
	void SetWalkAction(function<void()> action)
	{
		WalkAction_ = action;
	}

	/// @brief 止まった際のアクション設定
	/// @param action アクション
	void SetIdleAction(function<void()> action)
	{
		IdleAction_ = action;
	}

	/// @brief ゴールに到達したとき待機する時間を設定
	/// @param time 待機する時間
	void SetIdleTime(float time)
	{
		idleTime_ = time;
	}

	/// @brief ルートをランダムに徘徊するか否か設定
	/// @param isRandom ルートをランダムに徘徊するか否か
	void SetRandomState(bool isRandom)
	{
		isRandom_ = isRandom;
	}

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ROUTE_WALKER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override {}

	/// @brief 次の移動ノードを決定
	void SearchNextGoal();

	/// @brief 移動
	void Moving();

	/// @brief 待機
	void Idle();

	RouteNode current_;
	RouteNode goal_;

	bool isMoving_;
	bool isRandom_;

	map<int, RouteNode> route_;

	optional<reference_wrapper<Transform>> transform_;					/// @brief 移動させるオブジェクト

	function<void()> WalkAction_;		/// @brief 歩いた際の追加アクション
	function<void()> IdleAction_;		/// @brief 止まった際の追加アクション

	float idleTime_;	/// @brief ゴールに到達したとき待機する時間

	Vector3 moveDir_;	/// @brief 移動方向
	float speed_;		/// @brief 移動速度

	int uniqueID_;
};



