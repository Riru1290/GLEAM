#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Cylinder;

/// @brief �V�����_�[�����_���[(�C���X�^���V���O�`��)
class CylinderInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:

	[[nodiscard]] CylinderInstancingRenderer();
	~CylinderInstancingRenderer();

	/// @brief �`��C���X�^���X���ǉ�
	/// @param pos �V�����_�[���S���W
	/// @param height �V�����_�[�̍���
	/// @param r �V�����_�[�~�̔��a
	/// @param angle �p�x(�A�O���[)
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void Draw(Position3D pos, float height, float r, Vector3 angle, unsigned int color, float alpha, int fillFlag);


private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPHERE_RENDERER; }

	///// @brief ��{�萔�o�b�t�@�̐�
	//static constexpr int BASIC_CONSTANT_BUFFER_NUM = 3;

	///// @brief �����_���[�ɍŏ�����o�^����Ă���萔�o�b�t�@�̐�
	///// @return ��{�萔�o�b�t�@�̐�
	//[[nodiscard]] int GetBasicConstantBufferNum() const override { return BASIC_CONSTANT_BUFFER_NUM; }

	/// @brief �`�揀��
	void RenderBegin()override;

	/// @brief �`��I��
	void RenderEnd()override;

	/// @brief �`��I��(������)
	void RenderEndTrans()override {}

	unique_ptr<Cylinder> cylinder_;	/// @brief �`��ΏۃV�����_�[
};

