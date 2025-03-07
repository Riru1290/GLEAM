#pragma once
#include "../Object/Component/Component.h"
#include "../Object/Component/Renderer/Renderers/Single/MeshRenderer.h"
#include "../Object/Component/Renderer/Renderers/Single/2D/Object2DRenderer.h"
#include "../Object/Component/Renderer/Renderers/Single/2D/SpriteRenderer.h"
#include "../Object/Component/Renderer/Renderers/Single/Model/ModelRenderer.h"
#include "../Object/Component/Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "../Common/Mesh/Mesh.h"
#include "../Graphics/Material/Material.h"
#include "../Graphics/RenderManager.h"
#include "../Graphics/PipelineStateObject/PipelineStateObjectManager.h"

class Component;
class Transform;
class Quaternion;
class Texture;

class ModelRenderer;
class AnimationModelRenderer;
class Object2DRenderer;
class MeshRenderer;
class SpriteRenderer;

template <typename T>
concept RendererPossibleType =
is_same_v<T, ModelRenderer> ||
is_same_v<T, AnimationModelRenderer> ||
is_same_v<T, Object2DRenderer> ||
is_same_v<T, MeshRenderer>;

/// @brief ゲームに登場するUIの既定クラス
class UIObject : public enable_shared_from_this<UIObject>
{
public:


	UIObject();
	virtual ~UIObject();

	/// @brief オブジェクト識別用のキー取得
	/// @return オブジェクト識別用のキー
	[[nodiscard]] const string& GetObjectUniqueKey()const { return objectUniqueKey_; }

	template <RendererPossibleType T>
	void SetTexture(const Texture& texture)
	{
		auto renderer = RenderMng.CreateRenderer<T>(LAYER::UI);
		AddComponent(renderer);

		if constexpr (is_same_v<T, ModelRenderer>) {
			modelRenderer_ = *renderer;
		}
		else if constexpr (is_same_v<T, AnimationModelRenderer>) {
			animModelRenderer_ = *renderer;
		}
		else if constexpr (is_same_v<T, Object2DRenderer>) {
			object2DRenderer_ = *renderer;

			object2DRenderer_->get().SetTexture(texture);
			object2DRenderer_->get().SetTransform(transform_->get());
			object2DRenderer_->get().GetMesh().GetMaterial().SetPSOID(PSO_ID::DEFAULT_2D);
		}
		else if constexpr (is_same_v<T, MeshRenderer>) {
			meshRenderer_ = *renderer;

			meshRenderer_->get().Create2DMesh(texture);
			meshRenderer_->get().SetTransform(transform_->get());
			meshRenderer_->get().GetMesh().GetMaterial().SetPSOID(PSO_ID::UI_3D);
		}

		InitAfterSetTexture();
	}

	void SetTexture(const Texture& texture,int xNum,int yNum)
	{
		auto renderer = RenderMng.CreateRenderer<SpriteRenderer>(LAYER::UI);
		AddComponent(renderer);

		spriteRenderer_ = *renderer;

		spriteRenderer_->get().SetTexture(texture,xNum,yNum);
		spriteRenderer_->get().SetTransform(transform_->get());

		InitAfterSetTexture();
	}

	/// @brief 繰り返し初期化処理
	void Init();

	/// @brief 基本的な更新処理
	void Update();

	/// @brief 自分自身のshared_ptrを返す
	/// @return Objectのshared_ptr
	shared_ptr<UIObject> GetPtr() { return shared_from_this(); }

	/// @brief コンポーネント追加
	/// @param component コンポーネント
	/// @param overwrite 同一コンポーネントを上書きするか
	void AddComponent(shared_ptr<Component> component, bool overwrite = false);

	/// @brief 指定コンポーネントを取得
	/// @tparam T コンポーネントクラス
	/// @return 指定されたコンポーネント
	template<class T,
		enable_if_t<
		is_base_of_v<const Component, T>,
		nullptr_t> = nullptr>
	[[nodiscard]] T& GetComponent() const
	{
		// 指定されたコンポーネントが含まれているか確認
		for (auto& [id, registerCom] : components_)
		{
			// クラス名を比較
			if (typeid(*registerCom) != typeid(T))continue;

			return *static_pointer_cast<T>(registerCom);
		}

		// 見つからなかった場合、例外をスロー
		throw runtime_error("指定したコンポーネントが存在しません");
	}

	/// @brief すべてのコンポーネントのアクティブ状態をセット
	/// @param isActive アクティブ状態
	void SetActiveStateToAllComponents(bool isActive);

	/// @brief トランスフォーム取得
	/// @return トランスフォーム
	[[nodiscard]] const Transform& GetTransform()const { return transform_->get(); }

	/// @brief トランスフォーム設定
	/// @param transform トランスフォーム
	void SetTransform(Transform transform);

	/// @brief 座標設定
	/// @param pos 座標
	void SetPosition(Position3D pos);

	/// @brief 回転設定
	/// @param qua 回転
	void SetRotation(Quaternion qua);

	/// @brief 大きさ設定
	/// @param scale 大きさ
	void SetScale(Scale3D scale);

protected:


	/// @brief 繰り返し初期化処理
	virtual void InitUI() {}


	/// @brief 基本的な更新処理
	virtual void UpdateUI() = 0;

	/// @brief テクスチャ設定後の初期化
	virtual void InitAfterSetTexture() = 0;


	map<COMPONENT_ID, shared_ptr<Component>> components_;	/// @brief コンポーネント

	// 基本コンポーネント--------------------

	optional<reference_wrapper<Transform>> transform_;			/// @brief トランスフォーム情報

	optional<reference_wrapper<ModelRenderer>> modelRenderer_;
	optional<reference_wrapper<AnimationModelRenderer>> animModelRenderer_;
	optional<reference_wrapper<Object2DRenderer>> object2DRenderer_;
	optional<reference_wrapper<MeshRenderer>> meshRenderer_;
	optional<reference_wrapper<SpriteRenderer>> spriteRenderer_;

	//---------------------------------------

private:

	string objectUniqueKey_;	/// @brief オブジェクトを識別する唯一のキー
};

