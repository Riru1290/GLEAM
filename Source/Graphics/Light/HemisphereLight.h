#pragma once

/// @brief �������C�g
struct HEMISPHERE_LIGHT
{
	Vector3 skyColor = { 0.0f,0.0f,0.0f };		/// @brief �V���F
	Vector3 groundColor = { 0.0f,0.0f,0.0f };	/// @brief �n�ʐF
	Vector3 groundNormal = { 0.0f,1.0f,0.0f };	/// @brief �n�ʖ@��
};
