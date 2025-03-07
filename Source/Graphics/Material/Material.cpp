#include "../../Resource/ResourceManager.h"
#include "../../Resource/Texture/Texture.h"
#include "../PipelineStateObject/PipelineStateObjectManager.h"
#include "../Shadow/ShadowRenderer.h"
#include "Material.h"

namespace
{
	const int DIFFUSEMAP_SIGNATURE_NO_3D			= (8);
	const int SPECULARMAP_SIGNATURE_NO_3D			= (9);
	const int NORMALMAP_SIGNATURE_NO_3D				= (10);
	const int AMBIENTOCCLUSIONMAP_SIGNATURE_NO_3D	= (11);
	const int SHADOWMAP_SIGNATURE_NO_3D				= (12);

	const int DIFFUSEMAP_SIGNATURE_NO_2D			= (2);
	const int SPECULARMAP_SIGNATURE_NO_2D			= (3);
	const int NORMALMAP_SIGNATURE_NO_2D				= (4);
	const int AMBIENTOCCLUSIONMAP_SIGNATURE_NO_2D	= (5);
	const int SHADOWMAP_SIGNATURE_NO_2D				= (6);
}

Material::Material(PSO_ID psoID) :
	psoID_(psoID)
{
	diffuseMap_ = ResourceMng.LoadAdditionalTexture("");

	if (Is3DPipelineState(psoID_))
	{
		diffuseMapSignatureNo_ = DIFFUSEMAP_SIGNATURE_NO_3D;
		specularMapSignatureNo_ = SPECULARMAP_SIGNATURE_NO_3D;
		normalMapSignatureNo_ = NORMALMAP_SIGNATURE_NO_3D;
		ambientOcclusionMapSignatureNo_ = AMBIENTOCCLUSIONMAP_SIGNATURE_NO_3D;
		shadowMapSignatureNo_ = SHADOWMAP_SIGNATURE_NO_3D;
	}
	else
	{
		diffuseMapSignatureNo_ = DIFFUSEMAP_SIGNATURE_NO_2D;
		specularMapSignatureNo_ = SPECULARMAP_SIGNATURE_NO_2D;
		normalMapSignatureNo_ = NORMALMAP_SIGNATURE_NO_2D;
		ambientOcclusionMapSignatureNo_ = AMBIENTOCCLUSIONMAP_SIGNATURE_NO_2D;
		shadowMapSignatureNo_ = SHADOWMAP_SIGNATURE_NO_2D;
	}
}

Material::Material(XMFLOAT4 color) :
	psoID_(PSO_ID::PRIMITIVE_3D)
{
	diffuseMap_ = ResourceMng.RegisterTextureResource(
		to_string(color.x) + to_string(color.y) + to_string(color.z) + to_string(color.w),
		move(make_unique<Texture>(color)));

	if (Is3DPipelineState(psoID_))
	{
		diffuseMapSignatureNo_ = DIFFUSEMAP_SIGNATURE_NO_3D;
		specularMapSignatureNo_ = SPECULARMAP_SIGNATURE_NO_3D;
		normalMapSignatureNo_ = NORMALMAP_SIGNATURE_NO_3D;
		ambientOcclusionMapSignatureNo_ = AMBIENTOCCLUSIONMAP_SIGNATURE_NO_3D;
		shadowMapSignatureNo_ = SHADOWMAP_SIGNATURE_NO_3D;
	}
	else
	{
		diffuseMapSignatureNo_ = DIFFUSEMAP_SIGNATURE_NO_2D;
		specularMapSignatureNo_ = SPECULARMAP_SIGNATURE_NO_2D;
		normalMapSignatureNo_ = NORMALMAP_SIGNATURE_NO_2D;
		ambientOcclusionMapSignatureNo_ = AMBIENTOCCLUSIONMAP_SIGNATURE_NO_2D;
		shadowMapSignatureNo_ = SHADOWMAP_SIGNATURE_NO_2D;
	}
}

Material::~Material()
{
}

void Material::SetPSOID(PSO_ID psoID)
{
	psoID_ = psoID;

	if (Is3DPipelineState(psoID_))
	{
		diffuseMapSignatureNo_ = DIFFUSEMAP_SIGNATURE_NO_3D;
		specularMapSignatureNo_ = SPECULARMAP_SIGNATURE_NO_3D;
		normalMapSignatureNo_ = NORMALMAP_SIGNATURE_NO_3D;
		ambientOcclusionMapSignatureNo_ = AMBIENTOCCLUSIONMAP_SIGNATURE_NO_3D;
		shadowMapSignatureNo_ = SHADOWMAP_SIGNATURE_NO_3D;
	}
	else
	{
		diffuseMapSignatureNo_ = DIFFUSEMAP_SIGNATURE_NO_2D;
		specularMapSignatureNo_ = SPECULARMAP_SIGNATURE_NO_2D;
		normalMapSignatureNo_ = NORMALMAP_SIGNATURE_NO_2D;
		ambientOcclusionMapSignatureNo_ = AMBIENTOCCLUSIONMAP_SIGNATURE_NO_2D;
		shadowMapSignatureNo_ = SHADOWMAP_SIGNATURE_NO_2D;
	}
}

void Material::SetMaterial()
{
	// PSOをセット
	PSOMng.SetPSO(psoID_);

	// 拡散反射光
	if (diffuseMap_.has_value())
	{
		diffuseMap_->get().SetTexture(diffuseMapSignatureNo_);
	}

	// 鏡面反射光
	if (specularMap_.has_value())
	{
		specularMap_->get().SetTexture(specularMapSignatureNo_);
	}

	// ノーマルマップ
	if (normalMap_.has_value())
	{
		normalMap_->get().SetTexture(normalMapSignatureNo_);
	}

	// アンビエントオクルージョン
	if (ambientOcclusionMap_.has_value())
	{
		ambientOcclusionMap_->get().SetTexture(ambientOcclusionMapSignatureNo_);
	}

	Shadow.SetDepthTexture(shadowMapSignatureNo_);

}
