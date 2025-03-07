#pragma once
#include "../PipelineStateObject/PipelineStateObjectID.h"

class Texture;

class Material
{
public:

	[[nodiscard]] Material(PSO_ID psoID);
	[[nodiscard]] Material(XMFLOAT4 color);
	~Material();

	/// @brief �g�p����PSOID���Z�b�g
	/// @param posID PipelineStateObjectID
	void SetPSOID(PSO_ID psoID);

	/// @brief �g�p����g�U���ˌ��摜���Z�b�g
	/// @param texture �e�N�X�`��
	void SetDiffuseMap(const Texture& texture) { diffuseMap_ = cref(texture); }

	/// @brief �g�p���鋾�ʔ��ˌ��摜���Z�b�g
	/// @param texture �e�N�X�`��
	void SetSpecularMap(const Texture& texture) { specularMap_ = cref(texture); }

	/// @brief �g�p����@���}�b�v�摜���Z�b�g
	/// @param texture �e�N�X�`��
	void SetNormalMap(const Texture& texture) { normalMap_ = cref(texture); }

	/// @brief �g�p����AO�}�b�v�摜���Z�b�g
	/// @param texture �e�N�X�`��
	void SetAmbientOcclusionMap(const Texture& texture) { ambientOcclusionMap_ = cref(texture); }

	/// @brief PSO,�e�N�X�`���̃Z�b�g
	void SetMaterial();

private:

	PSO_ID psoID_;								/// @brief �g�p����PSO

	optional<reference_wrapper<const Texture>> diffuseMap_;				/// @brief �g�U���ˌ�
	optional<reference_wrapper<const Texture>> specularMap_;			/// @brief ���ʔ��ˌ�
	optional<reference_wrapper<const Texture>> normalMap_;				/// @brief �@���}�b�v
	optional<reference_wrapper<const Texture>> ambientOcclusionMap_;	/// @brief AO�}�b�v


	int diffuseMapSignatureNo_;
	int specularMapSignatureNo_;
	int normalMapSignatureNo_;
	int ambientOcclusionMapSignatureNo_;
	int shadowMapSignatureNo_;


};

