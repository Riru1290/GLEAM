#pragma once
#include "../../../Renderer.h"

class Model;
class Transform;

struct BoundingSphere;

/// @brief �A�j���[�V�������f�������_���[
class AnimationModelRenderer final : public Renderer
{
public:

	[[nodiscard]] AnimationModelRenderer();
	~AnimationModelRenderer();

	/// @brief �`��Ώۃ��f���ݒ�
	/// @param model �`��Ώۃ��f��
	void SetModel(const Model& model);

	/// @brief �`��Ώۃ��f���g�����X�t�H�[���ݒ�
	/// @param transform �`��Ώۃ��f���g�����X�t�H�[��
	void SetTransform(Transform& transform);

	/// @brief ���f���̓����x���Z�b�g
	void SetModelAlpha(float alpha);

	/// @brief ���f���Ƀ��[�J���s����Z�b�g
	/// @param matrix ���[�J���s��(�ʒu��]�傫��)
	void SetModelLocalTransformMatrix(const XMMATRIX& matrix);

	/// @brief ���f���Ƀ��[���h�s����Z�b�g
	/// @param matrix ���[���h�s��(�ʒu��]�傫��)
	void SetModelWorldTransformMatrix(const XMMATRIX& matrix);

	/// @brief ���f���Ƀ{�[���s����Z�b�g
	/// @param matrix �{�[���s��(�A�j���[�V�����s��)
	/// @param boneNum �{�[����
	void SetModelBoneMatrix(const XMMATRIX* matrix, int boneNum);

	/// @brief ���f���Ƀ{�[���s����Z�b�g
	/// @param matrix �{�[���s��(�A�j���[�V�����s��)
	/// @param boneNum �{�[���C���f�b�N�X
	void SetModelBoneMatrix(const XMMATRIX& matrix, int boneIndex);

	/// @brief 
	/// @param matrix 
	/// @param boneIndex 
	void SetAnimBoneMatrix(const XMMATRIX& matrix, int boneIndex);

	/// @brief ���f��
	/// @param matrix 
	/// @param boneIndex 
	void MultiplyMatrixToModelBoneAndChildren(const XMMATRIX& matrix, int boneIndex);

	/// @brief ���[�J���{�[���s��擾
	/// @param boneIndex �{�[���C���f�b�N�X
	/// @return ���[�J���{�[���s��
	[[nodiscard]] XMMATRIX GetLocalModelBoneMatrix(int boneIndex);

	/// @brief �O���[�o���{�[���s��擾
	/// @param boneIndex �{�[���C���f�b�N�X
	/// @return �O���[�o���{�[���s��
	[[nodiscard]] XMMATRIX GetGlobalModelBoneMatrix(int boneIndex);


	/// @brief ���[�J���A�j���[�V�����s��擾
	/// @param boneIndex �{�[���C���f�b�N�X
	/// @return ���[�J���A�j���[�V�����s��
	[[nodiscard]] XMMATRIX GetLocalAnimBoneMatrix(int boneIndex);

	/// @brief �O���[�o���A�j���[�V�����s��擾
	/// @param boneIndex �{�[���C���f�b�N�X
	/// @return �O���[�o���A�j���[�V�����s��
	[[nodiscard]] XMMATRIX GetGlobalAnimBoneMatrix(int boneIndex);

	/// @brief �O���[�o���{�[���s��擾
	/// @param boneIndex �{�[���C���f�b�N�X
	/// @return �O���[�o���{�[���s��
	[[nodiscard]] XMMATRIX GetGlobalBoneMatrix(int boneIndex);

	/// @brief �{�[���t���[���ω��s��擾
	/// @param boneIndex �{�[���C���f�b�N�X
	/// @return �{�[���t���[���ω��s��
	[[nodiscard]] XMMATRIX GetBoneFrameMatrix(int boneIndex);


	/// @brief ���f���Q��
	/// @return ���f��
	[[nodiscard]] const Model& GetModel()const { return model_->get(); }
	
private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::ANIMATION_MODEL_RENDERER; }

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

	/// @brief �{�[���s��o�b�t�@�쐬
	void CreateConstantBufferBoneMatrix();

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

	ComPtr<ID3D12Resource> boneConstBuffer_;			/// @brief �{�[���s��o�b�t�@
	int boneHeapNo_;									/// @brief �{�[���s��f�B�X�N���v�^�q�[�v�ԍ�
	XMMATRIX* boneMatrixMap_;							/// @brief �{�[���s��X�V�p�ϐ�

	XMMATRIX animMatrix[128];
	XMMATRIX boneFrameMatrix[128];

	vector<BoundingSphere> boundingSphereOrigin_;       /// @brief �o�E���f�B���O�X�t�B�A
	vector<BoundingSphere> boundingSphere_;             /// @brief �o�E���f�B���O�X�t�B�A

};

