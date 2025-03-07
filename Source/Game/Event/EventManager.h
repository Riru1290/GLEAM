#pragma once
#include "../../Common/Singleton/StaticSigleton.h"
#include "EventID.h"

#define EventMng EventManager::GetInstance()


/// @brief イベント管理
class EventManager final : public StaticSingleton<EventManager>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 実行
	void Execute();

	/// @brief 設定したイベントアクションをリセット
	void ResetEventActions();

	/// @brief イベント通知
	/// @param id 起こったイベント
	void Notify(EVENT_ID id);

	/// @brief イベントに対応したアクションを追加
	/// @param id イベントID
	/// @param func 実行するアクション
	void RegisterEventAction(EVENT_ID id, function<void()> func);

	/// @brief イベントに対応したアクションを追加(解放されない)
	/// @param id イベントID
	/// @param func 実行するアクション
	void RegisterFixedEventAction(EVENT_ID id, function<void()> func);

private:

	EventManager();
	~EventManager();
	PROOF_OF_STATIC_SINGLETON(EventManager);

	using EventCallback = vector<function<void()>>;

	unordered_map<EVENT_ID, EventCallback> eventActions_;
	unordered_map<EVENT_ID, EventCallback> fixedEventActions_;
};

