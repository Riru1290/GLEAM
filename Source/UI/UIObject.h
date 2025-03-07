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

/// @brief �Q�[���ɓo�ꂷ��UI�̊���N���X
class UIObject : public enable_shared_from_this<UIObject>
{
public:


	UIObject();
	virtual ~UIObject();

	/// @brief �I�u�W�F�N�g���ʗp�̃L�[�擾
	/// @return �I�u�W�F�N�g���ʗp�̃L�[
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

	/// @brief �J��Ԃ�����������
	void Init();

	/// @brief ��{�I�ȍX�V����
	void Update();

	/// @brief �������g��shared_ptr��Ԃ�
	/// @return Object��shared_ptr
	shared_ptr<UIObject> GetPtr() { return shared_from_this(); }

	/// @brief �R���|�[�l���g�ǉ�
	/// @param component �R���|�[�l���g
	/// @param overwrite ����R���|�[�l���g���㏑�����邩
	void AddComponent(shared_ptr<Component> component, bool overwrite = false);

	/// @brief �w��R���|�[�l���g���擾
	/// @tparam T �R���|�[�l���g�N���X
	/// @return �w�肳�ꂽ�R���|�[�l���g
	template<class T,
		enable_if_t<
		is_base_of_v<const Component, T>,
		nullptr_t> = nullptr>
	[[nodiscard]] T& GetComponent() const
	{
		// �w�肳�ꂽ�R���|�[�l���g���܂܂�Ă��邩�m�F
		for (auto& [id, registerCom] : components_)
		{
			// �N���X�����r
			if (typeid(*registerCom) != typeid(T))continue;

			return *static_pointer_cast<T>(registerCom);
		}

		// ������Ȃ������ꍇ�A��O���X���[
		throw runtime_error("�w�肵���R���|�[�l���g�����݂��܂���");
	}

	/// @brief ���ׂẴR���|�[�l���g�̃A�N�e�B�u��Ԃ��Z�b�g
	/// @param isActive �A�N�e�B�u���
	void SetActiveStateToAllComponents(bool isActive);

	/// @brief �g�����X�t�H�[���擾
	/// @return �g�����X�t�H�[��
	[[nodiscard]] const Transform& GetTransform()const { return transform_->get(); }

	/// @brief �g�����X�t�H�[���ݒ�
	/// @param transform �g�����X�t�H�[��
	void SetTransform(Transform transform);

	/// @brief ���W�ݒ�
	/// @param pos ���W
	void SetPosition(Position3D pos);

	/// @brief ��]�ݒ�
	/// @param qua ��]
	void SetRotation(Quaternion qua);

	/// @brief �傫���ݒ�
	/// @param scale �傫��
	void SetScale(Scale3D scale);

protected:


	/// @brief �J��Ԃ�����������
	virtual void InitUI() {}


	/// @brief ��{�I�ȍX�V����
	virtual void UpdateUI() = 0;

	/// @brief �e�N�X�`���ݒ��̏�����
	virtual void InitAfterSetTexture() = 0;


	map<COMPONENT_ID, shared_ptr<Component>> components_;	/// @brief �R���|�[�l���g

	// ��{�R���|�[�l���g--------------------

	optional<reference_wrapper<Transform>> transform_;			/// @brief �g�����X�t�H�[�����

	optional<reference_wrapper<ModelRenderer>> modelRenderer_;
	optional<reference_wrapper<AnimationModelRenderer>> animModelRenderer_;
	optional<reference_wrapper<Object2DRenderer>> object2DRenderer_;
	optional<reference_wrapper<MeshRenderer>> meshRenderer_;
	optional<reference_wrapper<SpriteRenderer>> spriteRenderer_;

	//---------------------------------------

private:

	string objectUniqueKey_;	/// @brief �I�u�W�F�N�g�����ʂ���B��̃L�[
};

