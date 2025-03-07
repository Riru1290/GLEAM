#pragma once
#include "../Object.h"

class AnimationController;
class AnimationModelRenderer;

/// @brief �A�j���[�V��������I�u�W�F�N�g�̊��N���X
class Actor : public Object
{
public:

	Actor(const string& uniqueKey);
	virtual ~Actor() = default;

protected:

	/// @brief �J��Ԃ�����������
	virtual void InitActor() {}

	/// @brief �`�揈��
	virtual void DrawActor() {}

	/// @brief ��{�I�ȍX�V����
	virtual void UpdateActor() = 0;

	/// @brief �ʏ�Update��ɑ��鏈��
	virtual void LateUpdateActor() {}

	/// @brief �J�����X�V��ɑ��鏈��
	virtual void OnCameraUpdateActor() {}

	/// @brief �A�j���[�V�����ݒ�
	virtual void AnimationSetting() = 0;

	// ��{�R���|�[�l���g--------------------

	optional<reference_wrapper<AnimationModelRenderer>> modelRenderer_;		/// @brief �A�j���[�V�������f�������_���[
	optional<reference_wrapper<AnimationController>> animationController_;	/// @brief �A�j���[�V�����R���g���[��

	//---------------------------------------

private:

	/// @brief �J��Ԃ�����������
	void InitObject() override final;

	/// @brief �`�揈��
	void DrawObject() override final;

	/// @brief ��{�I�ȍX�V����
	void UpdateObject() override final;

	/// @brief �ʏ�Update��ɑ��鏈��
	void LateUpdateObject() override final;

	/// @brief �J�����X�V��ɑ��鏈��
	void OnCameraUpdateObject() override final;

	/// @brief �A�j���[�V����������
	void InitAnimation();
};

