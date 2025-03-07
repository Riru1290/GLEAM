#pragma once
#include "../Component.h"

class InputManager;

/// @brief ���͕��@
enum class INPUT_PUSH_TYPE
{
	PRESS,
	TRRIGER,
	RELEASE
};

/// @brief ���̓R���g���[��
class InputController final : public Component
{
public:

	[[nodiscard]] InputController();
	~InputController() = default;

	/// @brief �L�[�Ɠ��͕��@�ɉ����������ǉ�
	/// @param key �L�[
	/// @param pushType ���͕��@
	/// @param func ����
	void AddFunction(const string& key, INPUT_PUSH_TYPE pushType, function<void()> func);

private:

	/// @brief �R���|�[�l���gID�擾
	/// @return �R���|�[�l���gID
	[[nodiscard]] COMPONENT_ID GetComponentID()const override { return COMPONENT_ID::INPUT_CONTROLLER; }

	/// @brief �X�V����
	void UpdateComponent() override;

	/// @brief �������
	void ReleaseComponent()override;

	optional<reference_wrapper<InputManager>> inputMng_;		/// @brief InputManager�Q��

	unordered_multimap<string, pair<INPUT_PUSH_TYPE, function<void()>>> controllList_;	/// @brief �R���g���[�����X�g

	unordered_map<INPUT_PUSH_TYPE, function<bool(const string&)>> isAction_;			/// @brief ���̓L�[�ɉ������֐��Ăяo��

};

