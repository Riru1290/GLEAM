#pragma once
#include "../Component.h"

class Transform;

/// @brief �w��I�u�W�F�N�g�B�Ƃ̋������v������
class DistanceChecker final : public Component
{
public:

	[[nodiscard]] DistanceChecker();
	~DistanceChecker() = default;

	/// @brief �v���^�[�Q�b�g�̒��ōł��߂��^�[�Q�b�g�Ƃ̋�����Ԃ�
	/// @return �ł��߂��^�[�Q�b�g�Ƃ̋���
	[[nodiscard]] float GetClosestTargetDistance();

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(const Transform& transform)
	{
		transform_ = transform;
	}

	/// @brief �Ǐ]�Ώےǉ�
	/// @param key �Ǐ]�Ώێ��ʃL�[
	/// @param target �Ǐ]�Ώ�
	void AddTargetTransform(const string& key ,const Transform& target)
	{
		targets_[key] = target;

		distances_[key] = 0.0f;
	}

	/// @brief �Ǐ]�Ώۍ폜
	/// @param key �Ǐ]�Ώێ��ʃL�[
	void DeleteTargetTransform(const string& key)
	{
		targets_.erase(key);
		distances_.erase(key);
	}

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::DISTANCE_CHECKER; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �������
	void ReleaseComponent() override {}

	/// @brief �����v��
	void Measurement();

	optional<reference_wrapper<const Transform>> transform_;							/// @brief ��I�u�W�F�N�g
	unordered_map<string,optional<reference_wrapper<const Transform>>> targets_;		/// @brief �v���Ώ�

	unordered_map<string, float> distances_;									/// @brief �v���ΏۂƂ̋���
};

