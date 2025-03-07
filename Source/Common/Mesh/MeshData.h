#pragma once
#include "../Vertex/VertexData.h"

/// @brief ���b�V���쐬���K�v�f�[�^
struct MESH_DATA
{
	vector<VERTEX_2D> verticesDefault2D;			/// @brief ���_�f�[�^2D
	vector<VERTEX_3D> verticesDefault3D;			/// @brief ���_�f�[�^3D
	vector<VERTEX_ANIMATION> verticesAnimation;		/// @brief ���_�f�[�^(�A�j���[�V����)
	vector<unsigned short>indices;					/// @brief �C���f�b�N�X�f�[�^

	string diffuseTexturePath;						/// @brief �g�U���ˌ�
	string specularTexturePath;						/// @brief ���ʔ��ˌ�
	string normalTexturePath;						/// @brief �m�[�}��
	string aoTexturePath;							/// @brief �A���r�G���g�I�N���[�W����
};
