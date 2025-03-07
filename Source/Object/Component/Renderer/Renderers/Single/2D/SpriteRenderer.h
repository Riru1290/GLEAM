#pragma once
#include "../../../Renderer.h"

class Mesh;
class Transform;
class Texture;

struct VERTEX_2D;

class SpriteRenderer final : public Renderer
{
public:

	[[nodiscard]] SpriteRenderer();
	~SpriteRenderer();

	/// @brief �e�N�X�`���ݒ�
	/// @param texture �e�N�X�`��
	/// @param xNum X������
	/// @param yNum Y������
	void SetTexture(const Texture& texture, int xNum, int yNum);

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
	[[nodiscard]] const Mesh& GetMesh()const;

	/// @brief �e�N�X�`�������T�C�Y���擾
	/// @return �e�N�X�`�������T�C�Y
	[[nodiscard]] const Size2D GetTextureSize() const { return texSize_; }

	/// @brief �w��ԍ��̉摜��`�悷��
	/// @param transform �\���ʒu
	/// @param no �ԍ�
	void RenderSprite(Transform transform, int no);

private:

	/// @brief �I�u�W�F�N�gID�擾
	/// @return �I�u�W�F�N�gID
	COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::SPRITE_RENDERER; }


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

	void RecreateVertex();

	optional<reference_wrapper<Transform>> transform_;	/// @brief �`��Ώۃ��f���g�����X�t�H�[��

	unique_ptr<Mesh> mesh_;								/// @brief �`��Ώۃ��b�V��

	vector<pair<Transform,int>> renderInfo_;			/// @brief �`�悷��摜�ԍ�

	Size2D texSize_;
	int divideXNum_;
	int divideYNum_;
	vector<UV> divideUV_;
	UV uvOffset_;

	ComPtr<ID3D12Resource> localWorldConstBuffer_;		/// @brief ���[�J���E���[���h�s��o�b�t�@
	int localWorldHeapNo_;								/// @brief ���[�J���E���[���h�s��f�B�X�N���v�^�q�[�v�ԍ�
	LW_MATRIX_DATA* localWorldMatrixMap_;				/// @brief ���[�J���E���[���h�s��X�V�p�ϐ�
};

