#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Box;

/// @brief �{�b�N�X�����_���[(�C���X�^���V���O�`��)
class BoxInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:

	[[nodiscard]] BoxInstancingRenderer();
	~BoxInstancingRenderer();

	/// @brief �`��C���X�^���X���ǉ�
	/// @param center ���S���W
	/// @param r ���a
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	
	/// @brief �`��C���X�^���X���ǉ�
	/// @param Pos1 �{�b�N�X���\�������_���̈�_�̍��W
	/// @param Pos2 �{�b�N�X���\�������_���̈�_�̍��W
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void Draw(Position3D Pos1, Position3D Pos2, unsigned int color, float alpha, int fillFlag);

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::BOX_RENDERER; }

	///// @brief ��{�萔�o�b�t�@�̐�
	//static constexpr int BASIC_CONSTANT_BUFFER_NUM = 3;

	///// @brief �����_���[�ɍŏ�����o�^����Ă���萔�o�b�t�@�̐�
	///// @return ��{�萔�o�b�t�@�̐�
	//[[nodiscard]] int GetBasicConstantBufferNum()const override { return BASIC_CONSTANT_BUFFER_NUM; };

	/// @brief �`�揀��
	void RenderBegin()override;

	/// @brief �`��I��
	void RenderEnd()override;

	/// @brief �`��I��(������)
	void RenderEndTrans()override {}

	unique_ptr<Box> box_;						/// @brief �`��Ώۃ{�b�N�X
};

