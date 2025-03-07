#pragma once
#include "../Common/Singleton/StaticSigleton.h"
#include "../Scene/SceneID.h"

#define UIMng UIManager::GetInstance()

class UIObject;

/// @brief UI�Ǘ�
class UIManager final : public StaticSingleton<UIManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief ���s
	void Execute();

	/// @brief UI�ǉ�
	/// @param key �C�ӂ̎��ʃL�[
	/// @param UI UI�I�u�W�F�N�g
	void AddUI(const string& key, shared_ptr<UIObject> UI);

	/// @brief UI�폜
	/// @param key �C�ӂ̎��ʃL�[
	void DeleteUI(const string& key);

	/// @brief �SUI�폜
	void ClearUIs();

	/// @brief �w��UI�擾
	/// @param key ���ʃL�[
	/// @return UI
	[[nodiscard]] weak_ptr<UIObject> GetUI(const string& key);

private:

	UIManager();
	~UIManager();
	PROOF_OF_STATIC_SINGLETON(UIManager);

	unordered_map<string, shared_ptr<UIObject>> UIObjects_;
};

