#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Object/Component/Renderer/Renderer.h"
#include "Layer.h"
#include "RendererID.h"

#define RenderMng RenderManager::GetInstance()

class DebugRenderer;
class Component;
class CBV_SRV_UAVHeap;


/// @brief 描画管理
class RenderManager final : public StaticSingleton<RenderManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief レンダラー作成
	/// @param rendererID レンダラーID
	/// @param layer レイヤーID
	/// @return レンダラー
	template<class T,
		enable_if_t<
		is_base_of_v<const Renderer, T>,
		nullptr_t> = nullptr>
	[[nodiscard]] shared_ptr<T> CreateRenderer(LAYER layer = LAYER::MODEL)
	{
		shared_ptr<T> renderer = nullptr;

		renderer = make_shared<T>();

		if (layer == LAYER::MODEL) {
			renderers_[layer].emplace_back(static_pointer_cast<Renderer>(renderer));
		}
		else{
			renderers2D_[layer].emplace_back(static_pointer_cast<Renderer>(renderer));
		}
	
		return renderer;
	}

	/// @brief 削除リストにレンダラーを追加
	/// @param renderer レンダラー
	void DeleteRenderer(shared_ptr<Component> renderer);

	/// @brief 描画準備
	void RenderBegin();

	/// @brief 描画終了
	void RenderEnd();

private:

	RenderManager();
	~RenderManager();
	PROOF_OF_STATIC_SINGLETON(RenderManager);

	/// @brief リストに追加されたレンダラーを削除
	void DeleteRendererFromList();

	/// @brief ビューポート,シザー矩形を設定
	void ViewportScissorRectSetting();

	/// @brief 深度バッファ作成
	void CreateDepthBuffer();

	/// @brief 通常描画
	void RenderDefault();

	/// @brief 半透明描画
	void RenderTrans();

	/// @brief 影描画(深度バッファ描画)
	void RenderShadow();

	/// @brief 2D描画
	void Render2D();

	map<LAYER, vector<weak_ptr<Renderer>>> renderers_;								/// @brief レンダラー配列
	map<LAYER, vector<weak_ptr<Renderer>>> renderers2D_;							/// @brief レンダラー配列
	unordered_map<RENDERER_ID, function<shared_ptr<Component>()>> MakeRenderer_;	/// @brief レンダラー生成関数

	vector<weak_ptr<Component>> deleteRendererList_;/// @brief 削除するレンダラーリスト

	D3D12_VIEWPORT viewport_;						/// @brief ビューポート
	D3D12_RECT scissorrect_;						/// @brief シザー短形

	ComPtr<ID3D12Resource> depthBuffer_;			/// @brief 深度バッファー
	int dsvHeapNo_;									/// @brief 深度バッファーヒープ番号


};
