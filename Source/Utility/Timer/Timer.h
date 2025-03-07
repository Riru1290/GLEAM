#pragma once
#include "../../Common/Singleton/StaticSigleton.h"

#define MainTimer Timer::GetInstance()

/// @brief 時間管理クラス
class Timer final : public StaticSingleton<Timer>
{
public:

	/// @brief 解放処理
	void Terminate() override;

	/// @brief 実行
	void Run();

	/// @brief タイマープロセスリセット
	void ResetMyTimer();

	/// @brief 指定のプロセスリセット
	/// @param key キー
	void Reset(string key);

	/// @brief 指定のプロセス開始
	/// @param key キー
	void Start(string key);

	/// @brief 指定のプロセス停止
	/// @param key キー
	void Stop(string key);

	/// @brief 指定のプロセスリスタート
	/// @param key キー
	void Restart(string key);

	/// @brief 指定のプロセス削除
	/// @param key キー
	void Delete(string key);


	/// @brief タイマープロセス追加
	/// @param key キー
	/// @param maxTime セットする時間
	/// @param start 自動でスタートさせるか
	void SetTimer(string key ,float maxTime , bool start = false);

	/// @brief 指定のプロセスの時間設定
	/// @param key キー
	/// @param time 時間
	void SetTime(string key ,float time);

	/// @brief 渡された値にデルタタイムを適用させる
	/// @param val デルタタイムを適用させたい値
	void MultDelta(float& val);

	/// @brief 渡された値にデルタタイムを適用させたものを返す
	/// @param val デルタタイムを適用させたい値
	/// @return デルタタイム適用後の値
	[[nodiscard]] float GetMultDelta(const float& val);


	/// @brief 時間を指定時間,指定のスピードで進ませる
	/// @param rate 時間経過速度
	/// @param time 時間経過速度を変化させる時間
	void SetWitchTimeOnMyTimer(float rate, float time);

	/// @brief デルタタイム取得
	/// @return デルタタイム
	[[nodiscard]] float GetDelta()const { return deltaTime_ * whichTimeRate_; }

	/// @brief 指定のプロセス現在時間取得
	/// @param key キー
	/// @return プロセス現在時間
	[[nodiscard]] float GetTime(string key)const;

	/// @brief 指定のプロセスタイマー時間取得
	/// @param key キー
	/// @return タイマー時間
	[[nodiscard]] float GetMaxTime(string key)const;

	// 現在の時間経過の割合を返す : 経過時間1秒,設定時間10秒の場合0.1が返される
	[[nodiscard]] float GetRate(string key)const;

	/// @brief 指定のプロセスが終了済みかどうかを返す
	/// @param key キー
	/// @return 指定のプロセスが終了済みかどうか
	bool IsEndTimer(string key)const;

	/// @brief 指定のプロセスが稼働中かどうかを返す
	/// @param key キー
	/// @return 指定のプロセスが稼働中かどうか
	bool InProgress(string key)const;

	/// @brief デルタタイム再計算
	void RecalculationDelta();

private:

	Timer();
	~Timer();
	PROOF_OF_STATIC_SINGLETON(Timer);

	float deltaTime_; /// @brief デルタタイム

	chrono::system_clock::time_point preTime_; /// @brief 1フレーム前時間

	/// @brief タイマープロセス
	struct Process
	{
		bool isRunning_ = false;	/// @brief 実行中かどうか
		bool isForward_ = false;	/// @brief 時間を増やしていくのか

		float time_ = 0.0f;			/// @brief 計測に使う時間
		float maxTime_ = 0.0f;		/// @brief タイマーセット時の時間

		bool isEnd_ = false;		/// @brief 一時停止させるか

		/// @brief 実行処理
		void Run()
		{
			if (!isRunning_)return;

			if (isForward_) {
				time_ += MainTimer.GetDelta();
			}
			else {
				time_ -= MainTimer.GetDelta();
				if (time_ <= 0.0f) {
					isEnd_ = true;
					isRunning_ = false;
					time_ = 0.0f;
				}
			}
		}

		/// @brief 初期化処理
		void Reset()
		{
			if (isForward_) {
				time_ = 0.0f;
			}
			else {
				time_ = maxTime_;
			}

			isRunning_ = false;
			isEnd_ = false;
		}
	};

	unordered_map<string, Process> processes_;	/// @brief プロセス

	float whichTimeRate_;	/// @brief 時間経過速度
	float whichTime_;		/// @brief 時間経過速度を変化させる時間

};

