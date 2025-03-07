#pragma once

/// @brief �V�[�����N���X
class SceneBase
{
public:

	SceneBase() = default;
	virtual ~SceneBase() = default;

	/// @brief ����������
	void Init();

	/// @brief �X�V����
	void Update();

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdate();

	/// @brief �`�揈��
	void Draw();

	/// @brief �������
	virtual void Terminate() = 0;

protected:

	/// @brief ����������
	virtual void InitScene() = 0;

	/// @brief �X�V����
	virtual void UpdateScene() = 0;

	/// @brief �J�����X�V��ɑ��鏈��
	virtual void OnCameraUpdateScene() = 0;

	/// @brief �`�揈��
	virtual void DrawScene() = 0;

	/// @brief �I�u�W�F�N�g�ݒ�
	virtual void ObjectSetting() = 0;

	/// @brief UI�ݒ�
	virtual void UISetting() = 0;

	/// @brief �J�����ݒ�
	virtual void CameraSetting() = 0;

	/// @brief ���C�g�ݒ�
	virtual void LightSetting() = 0;

	/// @brief �C�x���g�ݒ�
	virtual void EventSetting() = 0;
};

