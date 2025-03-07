#pragma once
#include "../SceneBase.h"

/// @brief ���U���g�V�[��
class ResultScene : public SceneBase
{
public:

	[[nodiscard]] ResultScene(bool isClear);
	~ResultScene() = default;

private:

	/// @brief ����������
	void InitScene() override;

	/// @brief �X�V����
	void UpdateScene() override;

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateScene() override;

	/// @brief �`�揈��
	void DrawScene() override;

	/// @brief �������
	void Terminate() override;

	/// @brief �I�u�W�F�N�g�ݒ�
	void ObjectSetting()override;

	/// @brief UI�ݒ�
	void UISetting() override;

	/// @brief �J�����ݒ�
	void CameraSetting() override;

	/// @brief ���C�g�ݒ�
	void LightSetting() override;

	/// @brief �C�x���g�ݒ�
	void EventSetting() override;

	bool isClear_;	/// @brief �N���A�������ǂ���
};

