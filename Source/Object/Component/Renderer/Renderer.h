#pragma once
#include "../Component.h"

/// @brief ���[�J��,���[���h�s��
struct LW_MATRIX_DATA
{
	XMMATRIX local;		/// @brief ���[�J���s��
	XMMATRIX world;		/// @brief ���[���h�s��
	float alpha = 1.0f;	/// @brief �����x
	Vector3 padding;
};

class Renderer : public Component
{
public:

	Renderer();
	virtual ~Renderer() = default;

	/// @brief �`�揀��
	virtual void RenderBegin() = 0;

	/// @brief �`��I��
	virtual void RenderEnd() = 0;

	/// @brief �`��I��(������)
	virtual void RenderEndTrans() = 0;

	/// @brief �e�`��(�[�x�o�b�t�@�`��)
	virtual void RenderShadow() {}

protected:

	/// @brief �X�V����
	virtual void UpdateRenderer() = 0;

	/// @brief �������
	virtual void ReleaseRenderer() = 0;
	
private:

	/// @brief �X�V����
	void UpdateComponent() {}

	/// @brief �X�V����
	void LateUpdateComponent() override;

	/// @brief �������
	void ReleaseComponent() override;

};

