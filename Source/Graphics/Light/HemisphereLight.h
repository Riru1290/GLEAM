#pragma once

/// @brief 半球ライト
struct HEMISPHERE_LIGHT
{
	Vector3 skyColor = { 0.0f,0.0f,0.0f };		/// @brief 天球色
	Vector3 groundColor = { 0.0f,0.0f,0.0f };	/// @brief 地面色
	Vector3 groundNormal = { 0.0f,1.0f,0.0f };	/// @brief 地面法線
};
