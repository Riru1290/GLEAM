#pragma once
#include "../../Component.h"

class Transform;

struct RouteNode
{
	int id;
	Position3D pos;
	vector<int> neighbors;
};

/// @brief �w�肳�ꂽ���[�g��p�j����
class RouteWalker final : public Component
{
public:

	[[nodiscard]] RouteWalker();
	~RouteWalker() = default;

	/// @brief �J�n�ʒu��ݒ�
	/// @param id �J�n�ʒuID
	void InitStart(int id);
	
	/// @brief �P���ȃ��[�g�����W�������Ƃɍ쐬
	/// @param route ���W�z��
	void CreateStraightRoute(vector<Position3D> route);

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief �ړ����x��ݒ�
	/// @param speed 
	void SetSpeed(float speed)
	{
		speed_ = speed;
	}

	/// @brief �ړ������擾
	/// @return �ړ�����
	[[nodiscard]] const Vector3& GetMoveDir()const { return moveDir_; }

	/// @brief �S�[�����W�擾
	/// @return �S�[�����W
	[[nodiscard]] Position3D GetGoalPosition()const { return goal_.pos; }

	/// @brief ���[�g�̒��̍ł��߂����W�擾
	/// @return �ł��߂����W
	[[nodiscard]] Position3D GetNeaPosition(const Position3D pos)const;

	/// @brief �w�肵�����W�ɍł��߂����[�g��̃m�[�h���S�[���Ƃ���
	[[nodiscard]] void SetNextNodeToNearestRoutePoint(const Position3D pos);

	/// @brief �������ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetWalkAction(function<void()> action)
	{
		WalkAction_ = action;
	}

	/// @brief �~�܂����ۂ̃A�N�V�����ݒ�
	/// @param action �A�N�V����
	void SetIdleAction(function<void()> action)
	{
		IdleAction_ = action;
	}

	/// @brief �S�[���ɓ��B�����Ƃ��ҋ@���鎞�Ԃ�ݒ�
	/// @param time �ҋ@���鎞��
	void SetIdleTime(float time)
	{
		idleTime_ = time;
	}

	/// @brief ���[�g�������_���ɜp�j���邩�ۂ��ݒ�
	/// @param isRandom ���[�g�������_���ɜp�j���邩�ۂ�
	void SetRandomState(bool isRandom)
	{
		isRandom_ = isRandom;
	}

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ROUTE_WALKER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override {}

	/// @brief ���̈ړ��m�[�h������
	void SearchNextGoal();

	/// @brief �ړ�
	void Moving();

	/// @brief �ҋ@
	void Idle();

	RouteNode current_;
	RouteNode goal_;

	bool isMoving_;
	bool isRandom_;

	map<int, RouteNode> route_;

	optional<reference_wrapper<Transform>> transform_;					/// @brief �ړ�������I�u�W�F�N�g

	function<void()> WalkAction_;		/// @brief �������ۂ̒ǉ��A�N�V����
	function<void()> IdleAction_;		/// @brief �~�܂����ۂ̒ǉ��A�N�V����

	float idleTime_;	/// @brief �S�[���ɓ��B�����Ƃ��ҋ@���鎞��

	Vector3 moveDir_;	/// @brief �ړ�����
	float speed_;		/// @brief �ړ����x

	int uniqueID_;
};



