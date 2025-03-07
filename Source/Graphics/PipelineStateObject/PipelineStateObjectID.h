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

	// インスタンシング描画
	PRIMITIVE_3D_INSTANCING,

	// UI
	UI_3D,

	// 独自
	FADE,
	LIMLIGHT,
	WAVE,
	TVSCREEN,
	TV,

	// 影
	SHADOW_DEFAULT,
	SHADOW_ANIMATION,
	SHADOW_2D,

	// ポストエフェクト
	PE_DEFAULT,
	PE_MONOCHROME,
	PE_REVERSE,
	PE_LOCKER,
};
