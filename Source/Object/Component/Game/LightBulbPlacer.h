#pragma once
#include "../Component.h"

class LightBulb;
class Transform;
class ModelRenderer;

/// @brief ���C�g��ݒu����
class LightBulbPlacer final : public Component
{
public:

	[[nodiscard]] LightBulbPlacer();
	~LightBulbPlacer();

	/// @brief ���O�ɐݒ肳�ꂽ�ꏊ�Ƀ��C�g��ݒu
	void PlaceLightBulb();

	/// @brief ���C�g��ݒu
	/// @param transform �ݒu���
	void PlaceLightBulb(Transform transform);

	/// @brief �ݒu�\�ȃ��C�g�̏���l��ݒ�
	/// @param maxNum 
	void SetLightBulbNumMax(int maxNum);

	/// @brief ���C�g�ݒu����ݒ�
	/// @param transform ���C�g�ݒu���
	void SetPlaceLightTransform(Transform transform) { placeLightTransform_ = transform; }

	/// @brief ���O�ɐݒ肳�ꂽ�w��̃��C�g�����
	void GetLightBulb();

	/// @brief ���C�g�������ݒ�
	/// @param key ���C�g������
	void SetGetLightBulbObjectUniqueKey(string key) { getLightKey_ = key; }

	void SetIsPossibleCheckFunction(function<bool()> func);

	/// @brief ���C�g�ێ������擾
	/// @return ���C�g�ێ���
	[[nodiscard]] const int& GetLightBulbNum()const { return lightBulbNum_; }

	void SetTempLightBulbModelTransState(bool isTrans);

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	virtual COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::LIGHTBULB_PLACER; }

	/// @brief �X�V����
	void UpdateComponent() override {}

	/// @brief �������
	void ReleaseComponent() override {}


	int lightBulbNum_;							/// @brief ���ݕێ����̃��C�g�̐�
	int placeLightBulbNum_;						/// @brief ���ݐݒu���̃��C�g�̐�
	int lightBulbNumMax_;						/// @brief �ݒu�\�ȃ��C�g�̐�
	unordered_map<string,bool> lightBulbKey_;	/// @brief ���C�g�Ǘ��L�[&�ݒu��Ԃ��ۂ�

	Transform placeLightTransform_;

	string getLightKey_;

	function<bool()> IsPossible_;	/// @brief ���C�g�ݒu��Ԃ��ǂ���

	optional<reference_wrapper<ModelRenderer>> temporaryLightBulbModel_;
	weak_ptr<LightBulb> temporaryLightBulb_;
};

