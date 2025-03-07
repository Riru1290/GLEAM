#pragma once
#include "../../Graphics/Light/LightController.h"
#include "../Object.h"

struct COL_HIT_DATA;

class DistanceChecker;

/// @brief �d���I�u�W�F�N�g
class LightBulb final : public Object
{
public:

	[[nodiscard]] LightBulb(const string& uniqueKey,const string& lightKey);
	[[nodiscard]] LightBulb(const string& uniqueKey);
	~LightBulb();

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	[[nodiscard]] OBJECT_ID GetObjectID()const override { return OBJECT_ID::LIGHTBULB; }

	/// @brief �J��Ԃ�����������
	void InitObject();

	/// @brief �`�揈��
	void DrawObject() override {}

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override {}

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateObject() override;

	/// @brief �R���C�_�[�ݒ�
	void ColliderSetting();

	void OnHitPlayerEye(COL_HIT_DATA data);

	string lightKey_;

	POINT_LIGHT lightInfo_;

	optional<reference_wrapper<DistanceChecker>> distanceChecker_;			/// @brief ��������

	bool isCreateLight_;
};

