#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "../RendererID.h"

#define DebugRender DebugRenderManager::GetInstance()

/// @brief �v���~�e�B�u�^�C�v
enum class PRIMITIVE_TYPE
{
	SPHERE,
	BOX,
	CAPSULE,
	CYLINDER,
};

class PrimitiveInstancingRenderer;

/// @brief �v���~�e�B�u�^�`��N���X
class DebugRenderManager final : public StaticSingleton<DebugRenderManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief ���̕`��
	/// @param center ���S���W
	/// @param r ���a
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void DrawSphere(Position3D center, float r, unsigned int color, float alpha, int fillFlag);

	/// @brief �`��C���X�^���X���ǉ�
	/// @param Pos1 �{�b�N�X���\�������_���̈�_�̍��W
	/// @param Pos2 �{�b�N�X���\�������_���̈�_�̍��W
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void DrawBox(Position3D Pos1, Position3D Pos2, unsigned int color, float alpha, int fillFlag);

	/// @brief �J�v�Z���`��
	/// @param pos1 �J�v�Z�����`�������_���̈�_�̍��W
	/// @param pos2 �J�v�Z�����`�������_���̈�_�̍��W
	/// @param r �J�v�Z���̕�
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void DrawCapsule(Position3D pos1, Position3D pos2, float r, unsigned int color, float alpha, int fillFlag);

	/// @brief �V�����_�[�`��
	/// @param pos �V�����_�[���S���W
	/// @param height �V�����_�[�̍���
	/// @param r �V�����_�[�~�̔��a
	/// @param angle �p�x(�A�O���[)
	/// @param color �F(0�`255)
	/// @param alpha �����x(0�`255)
	/// @param fillFlag ���C���t���[���\��
	void DrawCylinder(Position3D pos, float height, float r, Vector3 angle, unsigned int color, float alpha, int fillFlag);

private:

	DebugRenderManager();
	~DebugRenderManager();
	PROOF_OF_STATIC_SINGLETON(DebugRenderManager);

	unordered_map<PRIMITIVE_TYPE, shared_ptr<PrimitiveInstancingRenderer>> renderers_;		/// @brief �f�o�b�N�����_���[
};

