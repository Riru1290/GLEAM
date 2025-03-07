#pragma once
#include "../Component.h"
#include "../../../Utility/Math/Quaternion.h"

/// @brief �g�����X�t�H�[��(�ʒu,��],�傫��)
class Transform final : public Component
{
public:

	[[nodiscard]] Transform();
	[[nodiscard]] Transform(Position3D initPos, Quaternion initQua, Scale3D initScale = { 1.0f,1.0f,1.0f });
	~Transform() = default;

	Position3D pos;			/// @brief ���W
	Quaternion qua;			/// @brief ��]
	Scale3D scale;			/// @brief �傫��

	Position3D localPos;	/// @brief ���[�J�����W
	Quaternion localQua;	/// @brief ���[�J����]
	Scale3D localScale;		/// @brief ���[�J���傫��

	Position3D prePos;		/// @brief 1�t���[���O���W
	Quaternion preQua;		/// @brief 1�t���[���O��]
	Scale3D preScale;		/// @brief 1�t���[���O�傫��

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::TRANSFORM; }

	/// @brief �X�V����
	void UpdateComponent()override {};

	/// @brief �ʏ�Update�O�ɑ��鏈��
	void PreUpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override {};
};

