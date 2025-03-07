#pragma once
#include "../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../Resource/Model/Model.h"
#include "../Utility/Math/Quaternion.h"

/// @brief 形
enum class COLLISION_SHAPE_TYPE
{
	POINT = 1,
	LINE = 2,
	SPHERE = 4,
	BOX = 8,
	CAPSULE = 16,
	CYLINDER = 32,
	POLYGON = 64,
	MODEL = 128,
	RECT = 256
};

/// @brief コライダー基本情報
struct COL_INFO
{
	COL_INFO()
	{
		//UpdateAABB();
	}
	virtual ~COL_INFO() = default;

	/// @brief 初期化処理
	void Init()
	{
		InitColliderInfo();
		UpdateAABB();
	}

	/// @brief AABB座標取得
	/// @return first 最高座標 second 最低座標
	[[nodiscard]] const pair<Position3D, Position3D>& GetAABB() const { return AABBpos; }

	Position3D pos = {};
	Scale3D scale = { 1.0f,1.0f,1.0f };
	Quaternion qua = {};

protected:

	/// @brief 初期化処理
	virtual void InitColliderInfo() = 0;

	/// @brief AABB座標更新
	virtual void UpdateAABB() = 0;

	pair<Position3D, Position3D> AABBpos;
};

/// @brief 点コライダー情報
struct POINT_COL_INFO final : public COL_INFO
{
private:

	friend class PointCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override {}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		AABBpos = { pos ,pos };
	}
};

/// @brief 線分コライダー情報
struct LINE_COL_INFO final : public COL_INFO
{
public:

	Position3D posA;
	Position3D posB;

private:

	friend class LineCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override{}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		Position3D high = {};
		Position3D low = {};

		high.x = max(posA.x, posB.x);
		high.y = max(posA.y, posB.y);
		high.z = max(posA.z, posB.z);
		low.x = min(posA.x, posB.x);
		low.y = min(posA.y, posB.y);
		low.z = min(posA.z, posB.z);

		AABBpos = { high ,low };
	}
};

/// @brief 球体コライダー情報
struct SPHERE_COL_INFO final : public COL_INFO
{
public:

	float radius;	/// @brief 半径


private:

	friend class SphereCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{
		defRadius = radius;
	}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		Position3D high = pos + Vector3(radius, radius, radius);
		Position3D low = pos + Vector3(-radius, -radius, -radius);

		AABBpos = { high ,low };
	}

	float defRadius;	/// @brief 半径(初期値)
};

/// @brief 四角形コライダー情報
struct BOX_COL_INFO final : public COL_INFO
{
public:

	float width;	/// @brief 幅(X方向)
	float height;	/// @brief 高さ(Y方向)
	float depth;	/// @brief 奥行(Z方向)

private:

	friend class BoxCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{
		defWidth = width;
		defHeight = height;
		defDepth = depth;
	}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		Position3D high = pos + Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
		Position3D low = pos + Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f);

		AABBpos = { high ,low };
	}

	float defWidth;		/// @brief 幅(X方向)(初期値)
	float defHeight;	/// @brief 高さ(Y方向)(初期値)
	float defDepth;		/// @brief 奥行(Z方向)(初期値)
};

/// @brief カプセルコライダー情報
struct CAPSULE_COL_INFO final : public COL_INFO
{

	float height = 0.0f;	/// @brief 高さ
	float radius = 0.0f;	/// @brief 半径

	[[nodiscard]] Position3D GetTopPosition()const
	{
		Position3D topPos = {};
		topPos = pos + Position3D(0.0f, height / 2.0f, 0.0f);
		return topPos;
	}

	[[nodiscard]] Position3D GetBottomPosition()const
	{
		Position3D bottomPos = {};
		bottomPos = pos + Position3D(0.0f, -height / 2.0f, 0.0f);
		return bottomPos;
	}

private:

	friend class CapsuleCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{
		defHeight = height;
		defRadius = radius;
	}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		Position3D high = GetTopPosition() + Vector3(radius, radius, radius);
		Position3D low = GetBottomPosition() + Vector3(-radius, -radius, -radius);

		AABBpos = { high ,low };
	}

	float defHeight = 0.0f;			/// @brief 高さ(初期値)
	float defRadius = 0.0f;			/// @brief 半径(初期値)
};

/// @brief シリンダーコライダー情報
struct CYLINDER_COL_INFO final : public COL_INFO
{

	float height = 0.0f;	/// @brief 高さ
	float radius = 0.0f;	/// @brief 半径

	[[nodiscard]] Position3D GetTopPosition()const
	{
		Position3D topPos = {};
		topPos = pos + Position3D(0.0f, height / 2.0f, 0.0f);
		return topPos;
	}

	[[nodiscard]] Position3D GetBottomPosition()const
	{
		Position3D bottomPos = {};
		bottomPos = pos + Position3D(0.0f, -height / 2.0f, 0.0f);
		return bottomPos;
	}

private:

	friend class CylinderCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{
		defHeight = height;
		defRadius = radius;
	}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		Position3D high = GetTopPosition() + Vector3(radius, 0.0f, radius);
		Position3D low = GetBottomPosition() + Vector3(-radius, 0.0f, -radius);

		AABBpos = { high ,low };
	}

	float defHeight = 0.0f;			/// @brief 高さ(初期値)
	float defRadius = 0.0f;			/// @brief 半径(初期値)
};

/// @brief ポリゴンコライダー情報
struct POLYGON_COL_INFO final : public COL_INFO
{
public:

	Position3D tPos[3];	/// @brief ポリゴンを構成する3頂点
	Vector3 normal;		/// @brief 法線

private:

	friend struct MODEL_COL_INFO;
	friend struct MODEL_COL_CELL;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{

	}

	void UpdateAABB() override
	{
		Position3D high = tPos[0];
		Position3D low = tPos[0];

		for (const auto& t : tPos)
		{
			high.x = max(high.x, t.x);
			high.y = max(high.y, t.y);
			high.z = max(high.z, t.z);

			low.x = min(low.x, t.x);
			low.y = min(low.y, t.y);
			low.z = min(low.z, t.z);
		}

		AABBpos = { high ,low };
	}
};

struct MODEL_COL_CELL
{
public:

	/// @brief 初期化処理
	void Init()
	{
		for (auto& polygon : polygons)
		{
			polygon.Init();
		}
	}

	/// @brief AABB座標更新
	void UpdateAABB()
	{
		if (polygons.size() <= 0)return;

		for (auto& polygon : polygons)
		{
			polygon.UpdateAABB();
		}

		auto [high, low] = polygons[0].GetAABB();

		for (auto& polygon : polygons)
		{
			auto [pH, pL] = polygon.GetAABB();

			high.x = max(high.x, pH.x);
			high.y = max(high.y, pH.y);
			high.z = max(high.z, pH.z);

			low.x = min(low.x, pL.x);
			low.y = min(low.y, pL.y);
			low.z = min(low.z, pL.z);
		}

		AABBpos = { high ,low };
	}

	/// @brief AABB座標取得
	/// @return first 最高座標 second 最低座標
	[[nodiscard]] const pair<Position3D, Position3D>& GetAABB() const { return AABBpos; }

	vector<POLYGON_COL_INFO> polygons;	/// @brief モデルを構成するポリゴン

	pair<Position3D, Position3D> AABBpos;

};

/// @brief モデルコライダー情報
struct MODEL_COL_INFO final : public COL_INFO
{
public:

	void CreatePolygons(const Model& model)
	{
		auto& meshes = model.GetMeshes();
		vector<POLYGON_COL_INFO> polygons;

		for (auto& mesh : meshes)
		{
			auto vertices = static_cast<MeshDefault3D*>(mesh.get())->GetVertices();
			if (vertices.empty())continue;

			auto indices = mesh->GetIndices();

			for (int i = 0; i < static_cast<int>(indices.size()); i += 3)
			{
				POLYGON_COL_INFO polygon;
				auto& posA = vertices[indices[i]].pos;
				polygon.tPos[0] = { posA.x,posA.y, posA.z };

				auto& posB = vertices[indices[i + 1]].pos;
				polygon.tPos[1] = { posB.x,posB.y, posB.z };

				auto& posC = vertices[indices[i + 2]].pos;
				polygon.tPos[2] = { posC.x,posC.y, posC.z };

				polygon.pos =
					(((polygon.tPos[0] + polygon.tPos[1]) / 2.0f) + polygon.tPos[2]) / 2.0f;

				// エッジベクトルを計算
				Vector3 AtoB = polygon.tPos[1] - polygon.tPos[0];
				Vector3 AtoC = polygon.tPos[2] - polygon.tPos[0];

				// 法線を計算（ポリゴン法線を使用）
				polygon.normal = Cross(AtoB, AtoC).Normalized();

				polygon.UpdateAABB();

				polygons.emplace_back(polygon);
			}
		}

		// 並び替え
		auto compare = [](const POLYGON_COL_INFO& aPolygon, const POLYGON_COL_INFO& bPolygon) {

			const Vector3& a = aPolygon.pos;
			const Vector3& b = bPolygon.pos;
			if (a.x != b.x) return a.x > b.x;        // xで降順
			if (a.z != b.z) return a.z > b.z;        // zで降順
			return a.y > b.y;                        // yで降順
			};

		// ソート実行
		sort(polygons.begin(), polygons.end(), compare);

		polygonNum = static_cast<int>(polygons.size());

		CreateCell(polygons, 500.0f);

		Init();
	}

	
	vector<MODEL_COL_CELL> cells;		/// @brief モデルを構成するセル
	int polygonNum;						/// @brief ポリゴン数

private:

	friend class ModelCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{
		for (auto& cell : cells)
		{
			cell.Init();
		}
	}

	void UpdateAABB() override
	{
		if (cells.size() <= 0)return;

		for (auto& cell : cells)
		{
			cell.UpdateAABB();
		}

		auto [high, low] = cells[0].GetAABB();

		for (auto& cell : cells)
		{
			auto [pH, pL] = cell.GetAABB();

			high.x = max(high.x, pH.x);
			high.y = max(high.y, pH.y);
			high.z = max(high.z, pH.z);

			low.x = min(low.x, pL.x);
			low.y = min(low.y, pL.y);
			low.z = min(low.z, pL.z);
		}

		AABBpos = { high ,low };
	}

	/// @brief ポリゴン配列をもとにセルを作成
	/// @param polygons ポリゴン配列
	/// @param divisionSize XYZ分割サイズ
	void CreateCell(vector<POLYGON_COL_INFO>& polygons,float divisionSize)
	{
		if (polygons.size() <= 0)return;

		auto [high, low] = polygons[0].GetAABB();

		for (auto& pol : polygons)
		{
			auto [pH, pL] = pol.GetAABB();

			high.x = max(high.x, pH.x);
			high.y = max(high.y, pH.y);
			high.z = max(high.z, pH.z);

			low.x = min(low.x, pL.x);
			low.y = min(low.y, pL.y);
			low.z = min(low.z, pL.z);
		}

		// モデルのAABBをもとに空間分割を行う
		int divXNum = static_cast<int>(ceil((high.x - low.x) / divisionSize));
		int divYNum = static_cast<int>(ceil((high.y - low.y) / divisionSize));
		int divZNum = static_cast<int>(ceil((high.z - low.z) / divisionSize));

		Position3D base = high;

		for (int x = 0; x < divXNum; x++)
		{
			for (int y = 0; y < divYNum; y++)
			{
				for (int z = 0; z < divZNum; z++)
				{
					// セルのBB
					Position3D cellHigh =
					{ base.x - (divisionSize * x),
					  base.y - (divisionSize * y),
					  base.z - (divisionSize * z) };
					Position3D cellLow =
					{ base.x - (divisionSize * (x + 1)),
					  base.y - (divisionSize * (y + 1)),
					  base.z - (divisionSize * (z + 1)) };

					MODEL_COL_CELL cell;
					erase_if(polygons, [&cellHigh, &cellLow,&cell](POLYGON_COL_INFO& pol)
						{
							if (cellLow.x <= pol.pos.x && pol.pos.x <= cellHigh.x &&
								cellLow.y <= pol.pos.y && pol.pos.y <= cellHigh.y &&
								cellLow.z <= pol.pos.z && pol.pos.z <= cellHigh.z)
							{
								cell.polygons.emplace_back(pol);
								return true;
							}
							return false;
						}
					);


					cells.emplace_back(cell);
				}
			}
		}


	}

};

/// @brief 四角形コライダーを三角形ポリゴンに分割する関数
/// @param boxInfo 四角形コライダー情報
/// @return 分割された三角形ポリゴン情報の配列
static inline vector<POLYGON_COL_INFO> GeneratePolygonsFromBox(const BOX_COL_INFO& boxInfo)
{
	vector<POLYGON_COL_INFO> polygons;

	// 四角形の中心位置と幅、高さ、奥行
	const Position3D& center = boxInfo.pos;
	const float width = boxInfo.width;
	const float height = boxInfo.height;
	const float depth = boxInfo.depth;

	// 各頂点の計算
	Position3D vertices[8] = {
		// 前面
		{center.x - width / 2, center.y - height / 2, center.z - depth / 2}, // 0
		{center.x + width / 2, center.y - height / 2, center.z - depth / 2}, // 1
		{center.x + width / 2, center.y + height / 2, center.z - depth / 2}, // 2
		{center.x - width / 2, center.y + height / 2, center.z - depth / 2}, // 3

		// 背面
		{center.x - width / 2, center.y - height / 2, center.z + depth / 2}, // 4
		{center.x + width / 2, center.y - height / 2, center.z + depth / 2}, // 5
		{center.x + width / 2, center.y + height / 2, center.z + depth / 2}, // 6
		{center.x - width / 2, center.y + height / 2, center.z + depth / 2}  // 7
	};

	// 面ごとの三角形分割
	array<array<int, 3>, 12> indices = {
		// 前面
		array<int, 3>{0, 1, 2}, array<int, 3>{0, 2, 3},
		// 背面
		array<int, 3>{4, 6, 5}, array<int, 3>{4, 7, 6},
		// 左側面
		array<int, 3>{0, 3, 7}, array<int, 3>{0, 7, 4},
		// 右側面
		array<int, 3>{1, 5, 6}, array<int, 3>{1, 6, 2},
		// 上面
		array<int, 3>{3, 2, 6}, array<int, 3>{3, 6, 7},
		// 底面
		array<int, 3>{0, 4, 5}, array<int, 3>{0, 5, 1}
	};

	// 各三角形を生成
	for (const auto& idx : indices)
	{
		POLYGON_COL_INFO polygon;
		polygon.tPos[0] = vertices[idx[0]];
		polygon.tPos[1] = vertices[idx[1]];
		polygon.tPos[2] = vertices[idx[2]];

		// 法線を計算
		const Vector3 v1 = polygon.tPos[1] - polygon.tPos[0];
		const Vector3 v2 = polygon.tPos[2] - polygon.tPos[0];
		polygon.normal = Cross(v1, v2).Normalized();

		polygon.Init();

		polygons.push_back(polygon);
	}

	return polygons;
}

/// @brief 矩形（2D）コライダー情報
struct RECT_COL_INFO final : public COL_INFO
{
public:

	float width;	/// @brief 幅(X方向)
	float height;	/// @brief 高さ(Y方向)

private:

	friend class Rect2DCollider;

	/// @brief 初期化処理
	void InitColliderInfo() override
	{
		defWidth = width;
		defHeight = height;
	}

	/// @brief AABB座標更新
	void UpdateAABB() override
	{
		Position3D high = pos + Vector3(width / 2.0f, height / 2.0f, 0.0f);
		Position3D low = pos + Vector3(-width / 2.0f, -height / 2.0f, 0.0f);

		AABBpos = { high ,low };
	}

	float defWidth;		/// @brief 幅(X方向)(初期値)
	float defHeight;	/// @brief 高さ(Y方向)(初期値)
};
