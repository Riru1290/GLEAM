#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "PipelineStateObject.h"
#include "PipelineStateObjectID.h"

#define PSOCreator PipelineStateObjectCreator::GetInstance()

/// @brief PSOクリエイター
class PipelineStateObjectCreator final : public StaticSingleton<PipelineStateObjectCreator>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief PSO作成
	/// @param  PipelineStateObjectID
	/// @return PSO
	[[nodiscard]] PSO CreatePSO(PSO_ID psoID);

private:

	PipelineStateObjectCreator();
	~PipelineStateObjectCreator();
	PROOF_OF_STATIC_SINGLETON(PipelineStateObjectCreator);

	/// @brief デフォルトの3D用PSOを作成
	PSO CreatePSODefault3D();

	/// @brief デフォルトの2D用PSOを作成
	PSO CreatePSODefault2D();

	/// @brief プリミティブ3D用PSOを作成
	PSO CreatePSOPrimitive3D();

	/// @brief プリミティブ3Dインスタンス描画用PSOを作成
	PSO CreatePSOPrimitive3DInstancing();

	/// @brief UI3D用PSOを作成
	PSO CreatePSOUI3D();

	/// @brief アニメーション3D用PSOを作成
	PSO CreatePSOAnimation3D();

	/// @brief スプライト画像用PSOを作成
	PSO CreatePSOSprite2D();

	/// @brief リムライト用PSOを作成
	PSO CreatePSOLimlight();

	/// @brief 波用PSOを作成
	PSO CreatePSOWave();

	/// @brief フェード用PSOを作成
	PSO CreatePSOFade();

	/// @brief TVスクリーン用PSOを作成
	PSO CreatePSOTVScreen();

	/// @brief TV用PSOを作成
	PSO CreatePSOTV();

	/// @brief デフォルトシャドウPSOを作成
	PSO CreatePSOShadowDefault();

	/// @brief アニメーションシャドウPSOを作成
	PSO CreatePSOShadowAnimation();

	/// @brief 2DシャドウPSOを作成
	PSO CreatePSOShadow2D();

	/// @brief デフォルトポストエフェクトPSOを作成
	PSO CreatePSODefaultPE();

	/// @brief モノクロポストエフェクトPSOを作成
	PSO CreatePSOMonochromePE();

	/// @brief 反転ポストエフェクトPSOを作成
	PSO CreatePSOReversePE();

	/// @brief ロッカーポストエフェクトPSOを作成
	PSO CreatePSOLockerPE();

	/// @brief デフォルトのルートシグネチャ設定をセット
	/// @param dRange ディスクリプタレンジ
	/// @param rParam ルートパラメータ
	/// @param sDesc サンプラーデスク
	void SetDefaultRootSignatureDesc2D(
		CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc);

	/// @brief デフォルトのGPS設定をセット
	/// @param gpsDesc グラフィックスパイプラインステート
	/// @param rootSignature ルートシグネチャ
	/// @param inputLayout 頂点レイアウト
	/// @param inputLayoutNum 頂点レイアウトパラメータ数
	void SetDefaultGraphicsPipelineStateDesc2D(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	/// @brief デフォルトのルートシグネチャ設定をセット
	/// @param dRange ディスクリプタレンジ
	/// @param rParam ルートパラメータ
	/// @param sDesc サンプラーデスク
	void SetDefaultRootSignatureDesc3D(
		CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc);

	/// @brief デフォルトのルートシグネチャ設定をセット
	/// @param dRange ディスクリプタレンジ
	/// @param rParam ルートパラメータ
	/// @param sDesc サンプラーデスク
	void SetDefaultRootSignatureDescShadow(
		CD3DX12_DESCRIPTOR_RANGE* dRange, CD3DX12_ROOT_PARAMETER* rParam, CD3DX12_STATIC_SAMPLER_DESC* sDesc);

	/// @brief デフォルトのGPS設定をセット
	/// @param gpsDesc グラフィックスパイプラインステート
	/// @param rootSignature ルートシグネチャ
	/// @param inputLayout 頂点レイアウト
	/// @param inputLayoutNum 頂点レイアウトパラメータ数
	void SetDefaultGraphicsPipelineStateDesc3D(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	/// @brief デフォルトのGPS設定をセット
	/// @param gpsDesc グラフィックスパイプラインステート
	/// @param rootSignature ルートシグネチャ
	/// @param inputLayout 頂点レイアウト
	/// @param inputLayoutNum 頂点レイアウトパラメータ数
	/// @param vsPath 頂点シェーダパス
	void SetDefaultGraphicsPipelineStateDescShadow(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	/// @brief デフォルトのGPS設定をセット
	/// @param gpsDesc グラフィックスパイプラインステート
	/// @param rootSignature ルートシグネチャ
	/// @param inputLayout 頂点レイアウト
	/// @param inputLayoutNum 頂点レイアウトパラメータ数
	void SetDefaultGraphicsPipelineStateDescPostEffect(
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, ComPtr<ID3D12RootSignature> rootSignature,
		D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutNum);

	unordered_map<PSO_ID, function<PSO()>> CreatePSO_;	/// @brief PSO作成関数
};

