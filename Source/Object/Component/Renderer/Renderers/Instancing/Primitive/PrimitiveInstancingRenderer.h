#pragma once
#include "PrimitiveInstancingData.h"
#include "../../../Renderer.h"

/// @brief �C���X�^���V���O�����_���[
class PrimitiveInstancingRenderer : public Renderer
{
public:

	PrimitiveInstancingRenderer();
	virtual ~PrimitiveInstancingRenderer();

protected:

	/// @brief �萔�o�b�t�@�쐬
	void CreateConstantBuffer();

	vector<PRIMITIVE_DATA> primitiveData_;			/// @brief �C���X�^���X�f�[�^

	ComPtr<ID3D12Resource> instanceBuffer_;			/// @brief �C���X�^���X�f�[�^�o�b�t�@
	int instanceHeapNo_;							/// @brief �C���X�^���X�f�[�^�f�B�X�N���v�^�q�[�v�ԍ�
	PRIMITIVE_DATA* instanceMap_;					/// @brief �C���X�^���X�f�[�^�X�V�p�ϐ�

private:

	/// @brief �X�V����
	void UpdateRenderer() override {}

	/// @brief �������
	void ReleaseRenderer() override {}

};

