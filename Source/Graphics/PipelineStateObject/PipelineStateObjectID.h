#pragma once

/// @brief PipelineStateObjectID
enum class PSO_ID
{
	NONE,

	// 2D
	DEFAULT_2D,
	SPRITE2D,

	// 3D
	DEFAULT_3D,
	PRIMITIVE_3D,
	ANIMATION_MODEL,

	// �C���X�^���V���O�`��
	PRIMITIVE_3D_INSTANCING,

	// UI
	UI_3D,

	// �Ǝ�
	FADE,
	LIMLIGHT,
	WAVE,
	TVSCREEN,
	TV,

	// �e
	SHADOW_DEFAULT,
	SHADOW_ANIMATION,
	SHADOW_2D,

	// �|�X�g�G�t�F�N�g
	PE_DEFAULT,
	PE_MONOCHROME,
	PE_REVERSE,
	PE_LOCKER,
};
