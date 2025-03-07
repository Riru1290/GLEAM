#pragma once
#include "PrimitiveInstancingData.h"
#include "PrimitiveInstancingRenderer.h"

class Capsule;

/// @brief �J�v�Z�������_���[(�C���X�^���V���O�`��)
class CapsuleInstancingRenderer final : public PrimitiveInstancingRenderer
{
public:


	[[nodiscard]] CapsuleInstancingRenderer();
	~CapsuleInstancingRenderer();


	/// @brief �`��C���X�^���X���ǉ�
	/// @param pos1 �J�v�Z�����`�������_���̈�_�̍��W
	/// @param pos2 �J�v�Z�����`�������_���̈�_�̍��W
	/// @param radius �J�v�Z���̕�
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void Draw(Position3D pos1, Position3D pos2, float r, unsigned int color, float alpha, int fillFlag);


private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::CAPSULE_RENDERER; }

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

	/// @brief �J�v�Z���㔼�~�`��
	void DrawCapsuleTop();

	/// @brief �J�v�Z���~���`��
	void DrawCapsuleMiddle();

	/// @brief �J�v�Z�������~�`��
	void DrawCapsuleDown();

	/// @brief �㔼�~�萔�o�b�t�@�쐬
	void CreateConstantBufferSemiCircleTop();

	/// @brief �����~�萔�o�b�t�@�쐬
	void CreateConstantBufferSemiCircleDown();

	unique_ptr<Capsule> capsule_;				/// @brief �`��ΏۃJ�v�Z��

	vector<PRIMITIVE_DATA> capTopData_;			/// @brief �C���X�^���X�f�[�^(�J�v�Z���㔼�~�p)
	ComPtr<ID3D12Resource> capTopBuffer_;		/// @brief �C���X�^���X�f�[�^�o�b�t�@(�J�v�Z���㔼�~�p)
	int capTopHeapNo_;							/// @brief �C���X�^���X�f�[�^�f�B�X�N���v�^�q�[�v�ԍ�(�J�v�Z���㔼�~�p)
	PRIMITIVE_DATA* capTopMap_;					/// @brief �C���X�^���X�f�[�^�X�V�p�ϐ�(�J�v�Z���㔼�~�p)

	vector<PRIMITIVE_DATA> capDownData_;		/// @brief �C���X�^���X�f�[�^(�J�v�Z�������~�p)
	ComPtr<ID3D12Resource> capDownBuffer_;		/// @brief �C���X�^���X�f�[�^�o�b�t�@(�J�v�Z�������~�p)
	int capDownHeapNo_;							/// @brief �C���X�^���X�f�[�^�f�B�X�N���v�^�q�[�v�ԍ�(�J�v�Z�������~�p)
	PRIMITIVE_DATA* capDownMap_;				/// @brief �C���X�^���X�f�[�^�X�V�p�ϐ�(�J�v�Z�������~�p)
};
