#pragma once
#include "../../Component.h"

class Transform;
struct NavMesh;
struct NavMeshPolygon;

/// @brief �^�[�Q�b�g���i�r���b�V����A�����g�p���Ēǂ�
class AStarChaser final : public Component
{
public:

	[[nodiscard]] AStarChaser();
	~AStarChaser() = default;

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief �Ǐ]�Ώېݒ�
	/// @param target �Ǐ]�Ώ�
	void SetTargetTransform(Transform& target)
	{
		target_ = target;
	}

	/// @brief �i�r���b�V���ݒ�
	/// @param navMesh �i�r���b�V��
	void SetNavMesh(const NavMesh& navMesh)
	{
		navMesh_ = navMesh;
	}	
	
	/// @brief �ړ����x��ݒ�
	/// @param speed 
	void SetSpeed(float speed)
	{
		speed_ = speed;
	}

	/// @brief �������ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetMoveAction(function<void()> action)
	{
		MoveAction_ = action;
	}

	/// @brief �~�܂����ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetIdleAction(function<void()> action)
	{
		IdleAction_ = action;
	}

	void SetLimitDistance(float distance)
	{
		limitDistance_ = distance;
	}

	/// @brief �ړ������擾
	/// @return �ړ�����
	[[nodiscard]] const Vector3& GetMoveDir()const { return moveDir_; }

	/// @brief �Ǐ]�Ώە����擾
	/// @return �Ǐ]�Ώە���
	[[nodiscard]] Vector3 GetToTargetDir()const;

	/// @brief �S�[��������
	/// @return �S�[��������
	[[nodiscard]] const bool& IsGoal()const { return isGoal_; }

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ASTAR_CHASER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override;

	/// @brief �ړ�
	void Moving();

	/// @brief A*�o�H�T��
	/// @param startPos �J�n�ʒu
	/// @param goalPos �ŏI�ʒu
	/// @return �ŏI�ʒu�ւ̌o�H
	vector<Position3D> FindPathAStar(const Position3D& startPos, const Position3D& goalPos, vector<const NavMeshPolygon*>& polygons);

	
	vector<Position3D> GetOptimizedRoute(vector<Position3D>& route, vector<const NavMeshPolygon*>& polygons);
	
	optional<reference_wrapper<const NavMesh>> navMesh_;						/// @brief �T���p�i�r���b�V��	
	optional<reference_wrapper<Transform>> transform_;					/// @brief �ړ�������I�u�W�F�N�g
	optional<reference_wrapper<Transform>> target_;						/// @brief �Ǐ]�Ώ�

	Vector3 moveDir_;

	float speed_;
	float limitDistance_;

	function<void()> MoveAction_;		/// @brief�������ۂ̒ǉ��A�N�V����
	function<void()> IdleAction_;		/// @brief �~�܂����ۂ̒ǉ��A�N�V����

	bool isGoal_;	/// @brief �S�[��������
};

