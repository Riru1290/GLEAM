#pragma once
#include "../Common/Mesh/Meshes/MeshDefault3D.h"
#include "../Resource/Model/Model.h"
#include "../Utility/Math/Quaternion.h"

/// @brief �`
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

/// @brief �R���C�_�[��{���
struct COL_INFO
{
	COL_INFO()
	{
		//UpdateAABB();
	}
	virtual ~COL_INFO() = default;

	/// @brief ����������
	void Init()
	{
		InitColliderInfo();
		UpdateAABB();
	}

	/// @brief AABB���W�擾
	/// @return first �ō����W second �Œ���W
	[[nodiscard]] const pair<Position3D, Position3D>& GetAABB() const { return AABBpos; }

	Position3D pos = {};
	Scale3D scale = { 1.0f,1.0f,1.0f };
	Quaternion qua = {};

protected:

	/// @brief ����������
	virtual void InitColliderInfo() = 0;

	/// @brief AABB���W�X�V
	virtual void UpdateAABB() = 0;

	pair<Position3D, Position3D> AABBpos;
};

/// @brief �_�R���C�_�[���
struct POINT_COL_INFO final : public COL_INFO
{
private:

	friend class PointCollider;

	/// @brief ����������
	void InitColliderInfo() override {}

	/// @brief AABB���W�X�V
	void UpdateAABB() override
	{
		AABBpos = { pos ,pos };
	}
};

/// @brief �����R���C�_�[���
struct LINE_COL_INFO final : public COL_INFO
{
public:

	Position3D posA;
	Position3D posB;

private:

	friend class LineCollider;

	/// @brief ����������
	void InitColliderInfo() override{}

	/// @brief AABB���W�X�V
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

/// @brief ���̃R���C�_�[���
struct SPHERE_COL_INFO final : public COL_INFO
{
public:

	float radius;	/// @brief ���a


private:

	friend class SphereCollider;

	/// @brief ����������
	void InitColliderInfo() override
	{
		defRadius = radius;
	}

	/// @brief AABB���W�X�V
	void UpdateAABB() override
	{
		Position3D high = pos + Vector3(radius, radius, radius);
		Position3D low = pos + Vector3(-radius, -radius, -radius);

		AABBpos = { high ,low };
	}

	float defRadius;	/// @brief ���a(�����l)
};

/// @brief �l�p�`�R���C�_�[���
struct BOX_COL_INFO final : public COL_INFO
{
public:

	float width;	/// @brief ��(X����)
	float height;	/// @brief ����(Y����)
	float depth;	/// @brief ���s(Z����)

private:

	friend class BoxCollider;

	/// @brief ����������
	void InitColliderInfo() override
	{
		defWidth = width;
		defHeight = height;
		defDepth = depth;
	}

	/// @brief AABB���W�X�V
	void UpdateAABB() override
	{
		Position3D high = pos + Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
		Position3D low = pos + Vector3(-width / 2.0f, -height / 2.0f, -depth / 2.0f);

		AABBpos = { high ,low };
	}

	float defWidth;		/// @brief ��(X����)(�����l)
	float defHeight;	/// @brief ����(Y����)(�����l)
	float defDepth;		/// @brief ���s(Z����)(�����l)
};

/// @brief �J�v�Z���R���C�_�[���
struct CAPSULE_COL_INFO final : public COL_INFO
{

	float height = 0.0f;	/// @brief ����
	float radius = 0.0f;	/// @brief ���a

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

	/// @brief ����������
	void InitColliderInfo() override
	{
		defHeight = height;
		defRadius = radius;
	}

	/// @brief AABB���W�X�V
	void UpdateAABB() override
	{
		Position3D high = GetTopPosition() + Vector3(radius, radius, radius);
		Position3D low = GetBottomPosition() + Vector3(-radius, -radius, -radius);

		AABBpos = { high ,low };
	}

	float defHeight = 0.0f;			/// @brief ����(�����l)
	float defRadius = 0.0f;			/// @brief ���a(�����l)
};

/// @brief �V�����_�[�R���C�_�[���
struct CYLINDER_COL_INFO final : public COL_INFO
{

	float height = 0.0f;	/// @brief ����
	float radius = 0.0f;	/// @brief ���a

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

	/// @brief ����������
	void InitColliderInfo() override
	{
		defHeight = height;
		defRadius = radius;
	}

	/// @brief AABB���W�X�V
	void UpdateAABB() override
	{
		Position3D high = GetTopPosition() + Vector3(radius, 0.0f, radius);
		Position3D low = GetBottomPosition() + Vector3(-radius, 0.0f, -radius);

		AABBpos = { high ,low };
	}

	float defHeight = 0.0f;			/// @brief ����(�����l)
	float defRadius = 0.0f;			/// @brief ���a(�����l)
};

/// @brief �|���S���R���C�_�[���
struct POLYGON_COL_INFO final : public COL_INFO
{
public:

	Position3D tPos[3];	/// @brief �|���S�����\������3���_
	Vector3 normal;		/// @brief �@��

private:

	friend struct MODEL_COL_INFO;
	friend struct MODEL_COL_CELL;

	/// @brief ����������
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

	/// @brief ����������
	void Init()
	{
		for (auto& polygon : polygons)
		{
			polygon.Init();
		}
	}

	/// @brief AABB���W�X�V
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

	/// @brief AABB���W�擾
	/// @return first �ō����W second �Œ���W
	[[nodiscard]] const pair<Position3D, Position3D>& GetAABB() const { return AABBpos; }

	vector<POLYGON_COL_INFO> polygons;	/// @brief ���f�����\������|���S��

	pair<Position3D, Position3D> AABBpos;

};

/// @brief ���f���R���C�_�[���
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

				// �G�b�W�x�N�g�����v�Z
				Vector3 AtoB = polygon.tPos[1] - polygon.tPos[0];
				Vector3 AtoC = polygon.tPos[2] - polygon.tPos[0];

				// �@�����v�Z�i�|���S���@�����g�p�j
				polygon.normal = Cross(AtoB, AtoC).Normalized();

				polygon.UpdateAABB();

				polygons.emplace_back(polygon);
			}
		}

		// ���ёւ�
		auto compare = [](const POLYGON_COL_INFO& aPolygon, const POLYGON_COL_INFO& bPolygon) {

			const Vector3& a = aPolygon.pos;
			const Vector3& b = bPolygon.pos;
			if (a.x != b.x) return a.x > b.x;        // x�ō~��
			if (a.z != b.z) return a.z > b.z;        // z�ō~��
			return a.y > b.y;                        // y�ō~��
			};

		// �\�[�g���s
		sort(polygons.begin(), polygons.end(), compare);

		polygonNum = static_cast<int>(polygons.size());

		CreateCell(polygons, 500.0f);

		Init();
	}

	
	vector<MODEL_COL_CELL> cells;		/// @brief ���f�����\������Z��
	int polygonNum;						/// @brief �|���S����

private:

	friend class ModelCollider;

	/// @brief ����������
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

	/// @brief �|���S���z������ƂɃZ�����쐬
	/// @param polygons �|���S���z��
	/// @param divisionSize XYZ�����T�C�Y
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

		// ���f����AABB�����Ƃɋ�ԕ������s��
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
					// �Z����BB
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

/// @brief �l�p�`�R���C�_�[���O�p�`�|���S���ɕ�������֐�
/// @param boxInfo �l�p�`�R���C�_�[���
/// @return �������ꂽ�O�p�`�|���S�����̔z��
static inline vector<POLYGON_COL_INFO> GeneratePolygonsFromBox(const BOX_COL_INFO& boxInfo)
{
	vector<POLYGON_COL_INFO> polygons;

	// �l�p�`�̒��S�ʒu�ƕ��A�����A���s
	const Position3D& center = boxInfo.pos;
	const float width = boxInfo.width;
	const float height = boxInfo.height;
	const float depth = boxInfo.depth;

	// �e���_�̌v�Z
	Position3D vertices[8] = {
		// �O��
		{center.x - width / 2, center.y - height / 2, center.z - depth / 2}, // 0
		{center.x + width / 2, center.y - height / 2, center.z - depth / 2}, // 1
		{center.x + width / 2, center.y + height / 2, center.z - depth / 2}, // 2
		{center.x - width / 2, center.y + height / 2, center.z - depth / 2}, // 3

		// �w��
		{center.x - width / 2, center.y - height / 2, center.z + depth / 2}, // 4
		{center.x + width / 2, center.y - height / 2, center.z + depth / 2}, // 5
		{center.x + width / 2, center.y + height / 2, center.z + depth / 2}, // 6
		{center.x - width / 2, center.y + height / 2, center.z + depth / 2}  // 7
	};

	// �ʂ��Ƃ̎O�p�`����
	array<array<int, 3>, 12> indices = {
		// �O��
		array<int, 3>{0, 1, 2}, array<int, 3>{0, 2, 3},
		// �w��
		array<int, 3>{4, 6, 5}, array<int, 3>{4, 7, 6},
		// ������
		array<int, 3>{0, 3, 7}, array<int, 3>{0, 7, 4},
		// �E����
		array<int, 3>{1, 5, 6}, array<int, 3>{1, 6, 2},
		// ���
		array<int, 3>{3, 2, 6}, array<int, 3>{3, 6, 7},
		// ���
		array<int, 3>{0, 4, 5}, array<int, 3>{0, 5, 1}
	};

	// �e�O�p�`�𐶐�
	for (const auto& idx : indices)
	{
		POLYGON_COL_INFO polygon;
		polygon.tPos[0] = vertices[idx[0]];
		polygon.tPos[1] = vertices[idx[1]];
		polygon.tPos[2] = vertices[idx[2]];

		// �@�����v�Z
		const Vector3 v1 = polygon.tPos[1] - polygon.tPos[0];
		const Vector3 v2 = polygon.tPos[2] - polygon.tPos[0];
		polygon.normal = Cross(v1, v2).Normalized();

		polygon.Init();

		polygons.push_back(polygon);
	}

	return polygons;
}

/// @brief ��`�i2D�j�R���C�_�[���
struct RECT_COL_INFO final : public COL_INFO
{
public:

	float width;	/// @brief ��(X����)
	float height;	/// @brief ����(Y����)

private:

	friend class Rect2DCollider;

	/// @brief ����������
	void InitColliderInfo() override
	{
		defWidth = width;
		defHeight = height;
	}

	/// @brief AABB���W�X�V
	void UpdateAABB() override
	{
		Position3D high = pos + Vector3(width / 2.0f, height / 2.0f, 0.0f);
		Position3D low = pos + Vector3(-width / 2.0f, -height / 2.0f, 0.0f);

		AABBpos = { high ,low };
	}

	float defWidth;		/// @brief ��(X����)(�����l)
	float defHeight;	/// @brief ����(Y����)(�����l)
};
