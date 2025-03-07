#pragma once
#include "../../Renderer.h"
#include "../../../../../Common/AABB/BoundingSphere.h"

class Mesh;
class Transform;
class Texture;

/// @brief ���b�V�������_���[
class MeshRenderer final : public Renderer
{
public:

	[[nodiscard]] MeshRenderer();
	~MeshRenderer();

	/// @brief �ʃ��b�V���쐬
	/// @param texture �e�N�X�`��
	void Create2DMesh(const Texture& texture);

	/// @brief �ʃ��b�V���쐬
	/// @param size �T�C�Y
	void Create2DMesh(Size2D size);

	/// @brief �`��Ώۃ��b�V���ݒ�
	/// @param model �`��Ώۃ��b�V��
	void SetMesh(unique_ptr<Mesh> model);

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

	/// @brief ���f���Q��
	/// @return ���f��
	[[nodiscard]] Mesh& GetMesh()const { return *mesh_; }

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MESH_RENDERER; }

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

	/// @brief �o�E���f�B���O�X�t�B�A�f�[�^�X�V
	/// @param transform �ʒu���
	void UpdateBoundingSphere(const Transform& transform);

	/// @brief �I�u�W�F�N�g���J�����r���[�Ɋ܂܂�Ă��邩��BoundingSphere�Ń`�F�b�N
	/// @return �I�u�W�F�N�g���J�����r���[�Ɋ܂܂�Ă��邩
	[[nodiscard]] bool IsObjectInCameraFrustum(const BoundingSphere& sphere)const;

	optional<reference_wrapper<Transform>> transform_;	/// @brief �`��Ώۃ��f���g�����X�t�H�[��

	unique_ptr<Mesh> mesh_;	/// @brief �`��Ώۃ��b�V��

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ���[�J���E���[���h�s��o�b�t�@
	int localWorldHeapNo_;								/// @brief ���[�J���E���[���h�s��f�B�X�N���v�^�q�[�v�ԍ�
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ���[�J���E���[���h�s��X�V�p�ϐ�

	BoundingSphere boundingSphereOrigin_;       /// @brief �o�E���f�B���O�X�t�B�A
	BoundingSphere boundingSphere_;             /// @brief �o�E���f�B���O�X�t�B�A
};

