#pragma once
#include "../SceneBase.h"

class Object2DRenderer;


/// @brief ���r�[�V�[��
class LobbyScene : public SceneBase
{
public:

	[[nodiscard]] LobbyScene();
	~LobbyScene() = default;

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
	void ObjectSetting() override;

	/// @brief UI�ݒ�
	void UISetting() override;

	/// @brief �J�����ݒ�
	void CameraSetting() override;

	/// @brief ���C�g�ݒ�
	void LightSetting() override;

	/// @brief �C�x���g�ݒ�
	void EventSetting() override;


	// ���r�[��ʂ��ǂ���
	bool isLobby_;

	// ���\�[�X�n���h��
	int effectHandle_;
	int bgmHandle_;

#ifdef _DEBUG
	shared_ptr<Object2DRenderer> depth_;
#endif
};

