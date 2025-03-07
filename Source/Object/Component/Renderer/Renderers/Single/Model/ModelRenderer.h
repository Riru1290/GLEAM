#pragma once
#include "../../../Renderer.h"
#include "../../../../../../Common/AABB/BoundingSphere.h"

class Model;
class Transform;

/// @brief ���f�������_���[
class ModelRenderer final : public Renderer
{
public:

	[[nodiscard]] ModelRenderer();
	~ModelRenderer();

	/// @brief �`��Ώۃ��f���ݒ�
	/// @param model �`��Ώۃ��f��
	void SetModel(const Model& model);

	/// @brief �`��Ώۃ��f���g�����X�t�H�[���ݒ�
	/// @param transform �`��Ώۃ��f���g�����X�t�H�[��
	void SetTransform(Transform& transform);

	/// @brief ���f���Ƀ��[�J���s����Z�b�g
	/// @param matrix ���[�J���s��(�ʒu��]�傫��)
	void SetModelLocalTransformMatrix(const XMMATRIX& matrix);

	/// @brief ���f���Ƀ��[���h�s����Z�b�g
	/// @param matrix ���[���h�s��(�ʒu��]�傫��)
	void SetModelWorldTransformMatrix(const XMMATRIX& matrix);

	/// @brief �g�����X�t�H�[�������Ƀ��[���h�E���[�J���s����X�V
	/// @param transform �g�����X�t�H�[��
	void SetMeshTransformMatrixFromTransform(const Transform& transform);

	/// @brief ���f���̓����x���Z�b�g
	void SetModelAlpha(float alpha);

	/// @brief ���f���Q��
	/// @return ���f��
	[[nodiscard]] const Model& GetModel()const { return model_->get(); }

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::MODEL_RENDERER; }

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

	/// @brief �e�`��(�[�x�o�b�t�@�`��)
	void RenderShadow()override;

	/// @brief Transform�����ƂɃ��[�J���E���[���h�s����X�V
	void UpdateLocalWorldMatrix();

	/// @brief �萔�o�b�t�@�쐬
	void CreateConstantBuffer();

	/// @brief ���[�J���E���[���h�s��o�b�t�@�쐬
	void CreateConstantBufferLocalWorldMatrix();

	/// @brief �o�E���f�B���O�X�t�B�A�f�[�^�X�V
	/// @param transform �ʒu���
	void UpdateBoundingSphere(const Transform& transform);

	/// @brief �I�u�W�F�N�g���J�����r���[�Ɋ܂܂�Ă��邩��BoundingSphere�Ń`�F�b�N
	/// @return �I�u�W�F�N�g���J�����r���[�Ɋ܂܂�Ă��邩
	[[nodiscard]] bool IsObjectInCameraFrustum(const BoundingSphere& sphere)const;

	optional<reference_wrapper<const Model>> model_;	/// @brief �`��Ώۃ��f��
	optional<reference_wrapper<Transform>> transform_;	/// @brief �`��Ώۃ��f���g�����X�t�H�[��

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ���[�J���E���[���h�s��o�b�t�@
	int localWorldHeapNo_;								/// @brief ���[�J���E���[���h�s��f�B�X�N���v�^�q�[�v�ԍ�
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ���[�J���E���[���h�s��X�V�p�ϐ�

	vector<BoundingSphere> boundingSphereOrigin_;       /// @brief �o�E���f�B���O�X�t�B�A
	vector<BoundingSphere> boundingSphere_;             /// @brief �o�E���f�B���O�X�t�B�A
};

