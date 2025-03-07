#pragma once
#include "../PipelineStateObject/PipelineStateObjectID.h"
#include "../../Common/Singleton/StaticSigleton.h"

#define Shadow ShadowRenderer::GetInstance()

class ShadowRenderer final : public StaticSingleton<ShadowRenderer>
{
public:

	static constexpr int SHADOW_MAP_MAX_NUM = (10);

	/// @brief 解放処理
	void Terminate() override;

	void RenderBegin();

	/// @brief 深度バッファ撮影準備
	void SetRenderTargetDepth();

	void SetBeforeDepthTexture();
	void SetDepthTexture(int rootSignatureIndex);

	void SetShadowMapIndex(int rootIndex);

private:

	ShadowRenderer();
	~ShadowRenderer();
	PROOF_OF_STATIC_SINGLETON(ShadowRenderer);

	/// @brief 影用のバッファを作成
	void CreateShadowBuffer();

	/// @brief シャドウマップインデックス用のバッファを作成
	void CreateShadowMapIndexBuffer();

	/// @brief ビューポート,シザー矩形を設定
	void ViewportScissorRectSetting();

	ComPtr<ID3D12Resource> shadowBuffer_[SHADOW_MAP_MAX_NUM];		/// @brief リソース
	int srvHeapNo_[SHADOW_MAP_MAX_NUM];								/// @brief テクスチャディスクリプタヒープ番号
	int dsvHeapNo_[SHADOW_MAP_MAX_NUM];								/// @brief DSVディスクリプタヒープ番号

	ComPtr<ID3D12Resource> shadowMapIndexConstBuffer_;		/// @brief シャドウマップインデックス情報バッファ
	int shadowMapIndexHeapNo_;								/// @brief シャドウマップインデックス情報ディスクリプタヒープ番号
	int* shadowMapIndexMap_;								/// @brief バッファ更新用変数

	int setCount_;

	D3D12_VIEWPORT viewport_;						/// @brief ビューポート
	D3D12_RECT scissorrect_;						/// @brief シザー短形

	//// テスト
	//Position3D pos;
	//Position3D target;
	//Vector3 up;
	//ComPtr<ID3D12Resource> constBuffer_;
	//int heapNo_;
	//XMMATRIX* matrixMap_;
};

