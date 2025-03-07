#pragma once
#include "../../../Renderer.h"

class Mesh;
class Transform;
class Texture;

/// @brief 2D�����_���[
class Object2DRenderer final : public Renderer
{
public:

	[[nodiscard]] Object2DRenderer();
	~Object2DRenderer();

	/// @brief �e�N�X�`���ݒ�
	/// @param texture �e�N�X�`��
	void SetTexture(const Texture& texture);

	/// @brief ���b�V���쐬
	/// @param size ���b�V���T�C�Y
	void CreateMesh(Size2D size);

	/// @brief �`��Ώۃ��f���g�����X�t�H�[���ݒ�
	/// @param transform �`��Ώۃ��f���g�����X�t�H�[��
	void SetTransform(Transform& transform);

	/// @brief ���b�V���̓����x���Z�b�g
	void SetMeshAlpha(float alpha);

	/// @brief ���b�V���Ƀ��[�J���s����Z�b�g
	/// @param matrix ���[�J���s��(�ʒu��]�傫��)
	void SetMeshLocalTransformMatrix(const XMMATRIX& matrix);

	/// @brief ���b�V���Ƀ��[���h�s����Z�b�g
	/// @param matrix ���[���h�s��(�ʒu��]�傫��)
	void SetMeshWorldTransformMatrix(const XMMATRIX& matrix);

	/// @brief �g�����X�t�H�[�������Ƀ��[���h�E���[�J���s����X�V
	/// @param transform �g�����X�t�H�[��
	void SetMeshTransformMatrixFromTransform(const Transform& transform);

	/// @brief ���b�V���擾
	/// @return ���b�V��
	[[nodiscard]] Mesh& GetMesh()const { return *mesh_; }

	/// @brief �e�N�X�`���T�C�Y�擾
	/// @return �e�N�X�`���T�C�Y
	[[nodiscard]] const Size2D& GetTextureSize()const { return textureSize_; }

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::OBJECT2D_RENDERER; }


	/// @brief �X�V����
	void UpdateRenderer() override;

	/// @brief �������
	void ReleaseRenderer() override {}

	/// @brief �`�揀��
	void RenderBegin()override;

	/// @brief �`��I��
	void RenderEnd()override;

	/// @brief �`��I��(������)
	void RenderEndTrans()override;

	/// @brief �萔�o�b�t�@�쐬
	void CreateConstantBuffer();

	/// @brief ���[�J���E���[���h�s��o�b�t�@�쐬
	void CreateConstantBufferLocalWorldMatrix();

	/// @brief Transform�����ƂɃ��[�J���E���[���h�s����X�V
	void UpdateLocalWorldMatrix();

	optional<reference_wrapper<Transform>> transform_;	/// @brief �`��Ώۃ��f���g�����X�t�H�[��

	unique_ptr<Mesh> mesh_;			/// @brief �`��Ώۃ��b�V��

	Size2D textureSize_;			/// @brief �e�N�X�`���摜�T�C�Y

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ���[�J���E���[���h�s��o�b�t�@
	int localWorldHeapNo_;								/// @brief ���[�J���E���[���h�s��f�B�X�N���v�^�q�[�v�ԍ�
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ���[�J���E���[���h�s��X�V�p�ϐ�
};

