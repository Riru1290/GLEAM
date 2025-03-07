#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "EventID.h"

#define EventMng EventManager::GetInstance()


/// @brief �C�x���g�Ǘ�
class EventManager final : public StaticSingleton<EventManager>
{
public:

	/// @brief �������
	void Terminate() override;

	/// @brief ���s
	void Execute();

	/// @brief �ݒ肵���C�x���g�A�N�V���������Z�b�g
	void ResetEventActions();

	/// @brief �C�x���g�ʒm
	/// @param id �N�������C�x���g
	void Notify(EVENT_ID id);

	/// @brief �C�x���g�ɑΉ������A�N�V������ǉ�
	/// @param id �C�x���gID
	/// @param func ���s����A�N�V����
	void RegisterEventAction(EVENT_ID id, function<void()> func);

	/// @brief �C�x���g�ɑΉ������A�N�V������ǉ�(�������Ȃ�)
	/// @param id �C�x���gID
	/// @param func ���s����A�N�V����
	void RegisterFixedEventAction(EVENT_ID id, function<void()> func);

private:

	EventManager();
	~EventManager();
	PROOF_OF_STATIC_SINGLETON(EventManager);

	using EventCallback = vector<function<void()>>;

	unordered_map<EVENT_ID, EventCallback> eventActions_;
	unordered_map<EVENT_ID, EventCallback> fixedEventActions_;
};

