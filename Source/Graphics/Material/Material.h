#pragma once
#include "../PipelineStateObject/PipelineStateObjectID.h"

class Texture;

class Material
{
public:

	[[nodiscard]] Material(PSO_ID psoID);
	[[nodiscard]] Material(XMFLOAT4 color);
	~Material();

	/// @brief 使用するPSOIDをセット
	/// @param posID PipelineStateObjectID
	void SetPSOID(PSO_ID psoID);

	/// @brief 使用する拡散反射光画像をセット
	/// @param texture テクスチャ
	void SetDiffuseMap(const Texture& texture) { diffuseMap_ = cref(texture); }

	/// @brief 使用する鏡面反射光画像をセット
	/// @param texture テクスチャ
	void SetSpecularMap(const Texture& texture) { specularMap_ = cref(texture); }

	/// @brief 使用する法線マップ画像をセット
	/// @param texture テクスチャ
	void SetNormalMap(const Texture& texture) { normalMap_ = cref(texture); }

	/// @brief 使用するAOマップ画像をセット
	/// @param texture テクスチャ
	void SetAmbientOcclusionMap(const Texture& texture) { ambientOcclusionMap_ = cref(texture); }

	/// @brief PSO,テクスチャのセット
	void SetMaterial();

private:

	PSO_ID psoID_;								/// @brief 使用するPSO

	optional<reference_wrapper<const Texture>> diffuseMap_;				/// @brief 拡散反射光
	optional<reference_wrapper<const Texture>> specularMap_;			/// @brief 鏡面反射光
	optional<reference_wrapper<const Texture>> normalMap_;				/// @brief 法線マップ
	optional<reference_wrapper<const Texture>> ambientOcclusionMap_;	/// @brief AOマップ


	int diffuseMapSignatureNo_;
	int specularMapSignatureNo_;
	int normalMapSignatureNo_;
	int ambientOcclusionMapSignatureNo_;
	int shadowMapSignatureNo_;


};

