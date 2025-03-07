#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Object/Component/Renderer/Renderer.h"
#include "Layer.h"
#include "RendererID.h"

#define RenderMng RenderManager::GetInstance()

class DebugRenderer;
class Component;
class CBV_SRV_UAVHeap;


/// @brief �`��Ǘ�
class RenderManager final : public StaticSingleton<RenderManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief �����_���[�쐬
	/// @param rendererID �����_���[ID
	/// @param layer ���C���[ID
	/// @return �����_���[
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

	/// @brief �폜���X�g�Ƀ����_���[��ǉ�
	/// @param renderer �����_���[
	void DeleteRenderer(shared_ptr<Component> renderer);

	/// @brief �`�揀��
	void RenderBegin();

	/// @brief �`��I��
	void RenderEnd();

private:

	RenderManager();
	~RenderManager();
	PROOF_OF_STATIC_SINGLETON(RenderManager);

	/// @brief ���X�g�ɒǉ����ꂽ�����_���[���폜
	void DeleteRendererFromList();

	/// @brief �r���[�|�[�g,�V�U�[��`��ݒ�
	void ViewportScissorRectSetting();

	/// @brief �[�x�o�b�t�@�쐬
	void CreateDepthBuffer();

	/// @brief �ʏ�`��
	void RenderDefault();

	/// @brief �������`��
	void RenderTrans();

	/// @brief �e�`��(�[�x�o�b�t�@�`��)
	void RenderShadow();

	/// @brief 2D�`��
	void Render2D();

	map<LAYER, vector<weak_ptr<Renderer>>> renderers_;								/// @brief �����_���[�z��
	map<LAYER, vector<weak_ptr<Renderer>>> renderers2D_;							/// @brief �����_���[�z��
	unordered_map<RENDERER_ID, function<shared_ptr<Component>()>> MakeRenderer_;	/// @brief �����_���[�����֐�

	vector<weak_ptr<Component>> deleteRendererList_;/// @brief �폜���郌���_���[���X�g

	D3D12_VIEWPORT viewport_;						/// @brief �r���[�|�[�g
	D3D12_RECT scissorrect_;						/// @brief �V�U�[�Z�`

	ComPtr<ID3D12Resource> depthBuffer_;			/// @brief �[�x�o�b�t�@�[
	int dsvHeapNo_;									/// @brief �[�x�o�b�t�@�[�q�[�v�ԍ�


};
