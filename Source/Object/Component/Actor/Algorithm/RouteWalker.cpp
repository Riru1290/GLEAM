#ifdef _DEBUG
#include "../../../../Graphics/DebugRender/DebugRenderManager.h"
#include "../../../../../External/imgui/imgui.h"
#include "../../../../../External/imgui/imgui_impl_win32.h"
#include "../../../../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../../Common/Transform.h"
#include "RouteWalker.h"

RouteWalker::RouteWalker() : Component(),
isMoving_(false),
isRandom_(false),
idleTime_(0.0f),
uniqueID_(GetRand())
{
}

void RouteWalker::InitStart(int id)
{
	if (!route_.contains(id))return;

	isMoving_ = false;
	current_ = route_.at(id);

	transform_->get().pos = current_.pos;
}

void RouteWalker::CreateStraightRoute(vector<Position3D> route)
{
	route_.clear();

	for (int i = 0; i < static_cast<int>(route.size()); i++)
	{
		RouteNode node;
		node.id = i;
		node.pos = route[i];
		int front = (i + 1 > static_cast<int>(route.size()) - 1) ? 0 : i + 1;
		int back = (i - 1 < 0) ? static_cast<int>(route.size()) - 1 : i - 1;
		node.neighbors = { front ,back };

		route_.emplace(i, node);
	}
}

Position3D RouteWalker::GetNeaPosition(const Position3D pos) const
{
	float neaDis = D3D12_FLOAT32_MAX;
	Position3D nesPos = {};

	for (const auto& [id,node] : route_)
	{
		float dis = (pos - node.pos).Length();

		if (dis <= neaDis)
		{
			neaDis = dis;
			nesPos = node.pos;
		}
	}
	return nesPos;
}

void RouteWalker::SetNextNodeToNearestRoutePoint(const Position3D pos)
{
	float neaDis = D3D12_FLOAT32_MAX;
	int neaID = 0;

	for (const auto& [id, node] : route_)
	{
		float dis = (pos - node.pos).Length();

		if (dis <= neaDis)
		{
			neaDis = dis;
			neaID = id;
		}
	}

	isMoving_ = false;
	current_ = route_.at(neaID);
}

void RouteWalker::UpdateComponent()
{
	SearchNextGoal();

	if (MainTimer.InProgress("RouteWalker" + to_string(uniqueID_)))
	{
		Idle();
	}
	else
	{
		Moving();
	}
}

void RouteWalker::SearchNextGoal()
{
	if (isMoving_ || MainTimer.InProgress("RouteWalker" + to_string(uniqueID_)))return;

	if (isRandom_)
	{
		goal_ = route_.at(
			current_.neighbors[GetRand(0,static_cast<int>(current_.neighbors.size()))]);
	}
	else
	{
		goal_ = route_.at(current_.neighbors[0]);
	}

	moveDir_ = (goal_.pos - transform_->get().pos).Normalized();

	current_ = goal_;

	isMoving_ = true;
}

void RouteWalker::Moving()
{
	if (!isMoving_)return;

	bool isGoal = false;

	float movePow = MainTimer.GetMultDelta(speed_);

	float toGoalDistance = (goal_.pos - transform_->get().pos).Length();

	if (toGoalDistance <= movePow)
	{
		isGoal = true;
	}

	if (isGoal)
	{
		transform_->get().pos = goal_.pos;
		isMoving_ = false;

		MainTimer.SetTimer("RouteWalker" + to_string(uniqueID_), idleTime_,true);
	}
	else
	{
		transform_->get().pos += (moveDir_ * movePow);
	}

	WalkAction_();
}

void RouteWalker::Idle()
{
	IdleAction_();
}
