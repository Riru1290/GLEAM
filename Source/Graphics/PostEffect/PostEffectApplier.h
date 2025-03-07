#pragma once
#include "../PipelineStateObject/PipelineStateObjectID.h"
#include "../../Common/Singleton/StaticSigleton.h"

#define PostEffect PostEffectApplier::GetInstance()

enum class POSTEFFECT_ID
{
	DEFAULT,
	MONOCHROME,
	REVERSE,
	LOCKER,
};

class Mesh;

/// @brief ポストエフェクト適用
class PostEffectApplier final : public StaticSingleton<PostEffectApplier>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 深度バッファハンドルを設定
	/// @param depthHandle 深度バッファハンドル
	void SetDepthBufferHandle(D3D12_CPU_DESCRIPTOR_HANDLE depthHandle)
	{
		depthHandle_ = depthHandle;
	}

	/// @brief レンダーターゲットを設定
	void SetRenderTarget();

	/// @brief 適用させるポストエフェクトを予約
	/// @param id ポストエフェクトID
	void ReservePostEffect(POSTEFFECT_ID id);

	/// @brief ポストエフェクトを適用
	void ApplyPostEffect();

	/// @brief ポストエフェクト適用後スクリーンを描画
	void Render();

	ComPtr<ID3D12Resource> GetBuffer();

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();

private:

	PostEffectApplier();
	~PostEffectApplier();
	PROOF_OF_STATIC_SINGLETON(PostEffectApplier);

	/// @brief PSOID対応表を設定
	void PSOListSetting();

	/// @brief ポストエフェクト用のバッファを作成
	void CreatePostEffectBuffer();

	/// @brief スクリーン上を覆うメッシュを作成
	void CreateMesh();


	ComPtr<ID3D12Resource> postEffectBuffer_[2];	/// @brief リソース
	int srvHeapNo_[2];								/// @brief テクスチャディスクリプタヒープ番号
	int rtvHeapNo_[2];								/// @brief RTVディスクリプタヒープ番号

	int currentBufferNo_;							/// @brief 現在使用しているバッファ番号

	unordered_map<POSTEFFECT_ID,PSO_ID> psoList_;	/// @brief PSOID対応表

	vector<POSTEFFECT_ID> applyPosteffectList_;		/// @brief 適用させるポストエフェクト

	unique_ptr<Mesh> mesh_;							/// @brief スクリーン上を覆うメッシュ

	D3D12_CPU_DESCRIPTOR_HANDLE depthHandle_;
};

