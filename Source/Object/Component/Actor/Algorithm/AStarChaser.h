#pragma once
#include "../../Component.h"

class Transform;
struct NavMesh;
struct NavMeshPolygon;

/// @brief ターゲットをナビメッシュとA＊を使用して追う
class AStarChaser final : public Component
{
public:

	[[nodiscard]] AStarChaser();
	~AStarChaser() = default;

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief 追従対象設定
	/// @param target 追従対象
	void SetTargetTransform(Transform& target)
	{
		target_ = target;
	}

	/// @brief ナビメッシュ設定
	/// @param navMesh ナビメッシュ
	void SetNavMesh(const NavMesh& navMesh)
	{
		navMesh_ = navMesh;
	}	
	
	/// @brief 移動速度を設定
	/// @param speed 
	void SetSpeed(float speed)
	{
		speed_ = speed;
	}

	/// @brief 動いた際のアクション設定
	/// @param action アクション
	void SetMoveAction(function<void()> action)
	{
		MoveAction_ = action;
	}

	/// @brief 止まった際のアクション設定
	/// @param action アクション
	void SetIdleAction(function<void()> action)
	{
		IdleAction_ = action;
	}

	void SetLimitDistance(float distance)
	{
		limitDistance_ = distance;
	}

	/// @brief 移動方向取得
	/// @return 移動方向
	[[nodiscard]] const Vector3& GetMoveDir()const { return moveDir_; }

	/// @brief 追従対象方向取得
	/// @return 追従対象方向
	[[nodiscard]] Vector3 GetToTargetDir()const;

	/// @brief ゴールしたか
	/// @return ゴールしたか
	[[nodiscard]] const bool& IsGoal()const { return isGoal_; }

private:

	/// @brief コンポーネントID取得
	/// @return コンポーネントID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ASTAR_CHASER; }

	/// @brief 更新処理
	void UpdateComponent() override;

	/// @brief 解放処理
	void ReleaseComponent() override;

	/// @brief 移動
	void Moving();

	/// @brief A*経路探索
	/// @param startPos 開始位置
	/// @param goalPos 最終位置
	/// @return 最終位置への経路
	vector<Position3D> FindPathAStar(const Position3D& startPos, const Position3D& goalPos, vector<const NavMeshPolygon*>& polygons);

	
	vector<Position3D> GetOptimizedRoute(vector<Position3D>& route, vector<const NavMeshPolygon*>& polygons);
	
	optional<reference_wrapper<const NavMesh>> navMesh_;						/// @brief 探索用ナビメッシュ	
	optional<reference_wrapper<Transform>> transform_;					/// @brief 移動させるオブジェクト
	optional<reference_wrapper<Transform>> target_;						/// @brief 追従対象

	Vector3 moveDir_;

	float speed_;
	float limitDistance_;

	function<void()> MoveAction_;		/// @brief動いた際の追加アクション
	function<void()> IdleAction_;		/// @brief 止まった際の追加アクション

	bool isGoal_;	/// @brief ゴールしたか
};

