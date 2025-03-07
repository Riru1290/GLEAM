#pragma once
#include "../../Common/Mesh/MeshData.h"
#include "../../Common/Vertex/VertexData.h"

/// @brief �{�[���f�[�^
struct BONE_DATA
{
	string boneName;					/// @brief �{�[���̖��O

	vector<int> chiledlenBoneIndices;	/// @brief �q���̃C���f�b�N�X

	XMMATRIX transformLinkMat;			/// @brief �����p���s��
	XMMATRIX globalBindInverseMat;		/// @brief �����p���t�s��
};

/// @brief ���f���쐬���K�v�f�[�^
struct MODEL_DATA
{
	vector<MESH_DATA> meshes;	/// @brief ���b�V���f�[�^

	vector<BONE_DATA> bones;	/// @brief �{�[���f�[�^
};

/// @brief �A�j���[�V�����f�[�^
struct ANIMATION_DATA
{
	string animName;								/// @brief �A�j���[�V������

	map<string, vector<XMMATRIX>> animBoneMatrix;	/// @brief �{�[�����Ǝ��Ԃ��Ƃ̃A�j���[�V�����s��

	double start;									/// @brief �A�j���[�V�����J�n����(�b)
	double end;										/// @brief �A�j���[�V�����I������(�b)
	double frameRate;								/// @brief �A�j���[�V�����t���[�����[�g(�����ނ�60)
};
