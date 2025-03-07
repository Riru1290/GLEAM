#pragma once
#include "../SceneBase.h"
#include "../../Object/ObjectID.h"

class Object2DRenderer;

/// @brief �Q�[���v���C�V�[��
class PlayScene : public SceneBase
{
public:

	[[nodiscard]] PlayScene() = default;
	~PlayScene() = default;

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

#ifdef _DEBUG
	shared_ptr<Object2DRenderer> depth_;
#endif
};

