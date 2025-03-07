#include "Timer.h"

#ifdef _DEBUG

static float displayRefreshRate = 60.0f;

#endif

Timer::Timer()
{
	// デルタタイム
	preTime_ = chrono::system_clock::now();
	deltaTime_ = 0.0f;


	// 時間の経過速度
	whichTimeRate_ = 1.0f;
	whichTime_ = 0.0f;

#ifdef _DEBUG

	// ディスプレイのリフレッシュレートを取得
	DEVMODE devMode;
	ZeroMemory(&devMode, sizeof(devMode));
	devMode.dmSize = sizeof(devMode);

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

	displayRefreshRate = static_cast<float>(devMode.dmDisplayFrequency);

#endif
}

Timer::~Timer()
{
	processes_.clear();
}

void Timer::Terminate()
{
	processes_.clear();
}

void Timer::Run()
{
	// デルタタイム
	auto nowTime = chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		chrono::duration_cast<chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

#ifdef _DEBUG

	// デバッグ中はデルタタイムを固定
	//deltaTime_ = 1.0f / displayRefreshRate;

	//if (deltaTime_ >= 1.0f / displayRefreshRate)
	//{
	//	deltaTime_ = 1.0f / displayRefreshRate;
	//}

#endif

	for (auto& p : processes_) {
		p.second.Run();
	}

	// 時間の経過速度を変化させる
	if (whichTime_ > 0.0f) {
		whichTime_ -= deltaTime_;
		if (whichTime_ < 0.0f) {
			whichTime_ = 0.0f;
			whichTimeRate_ = 1.0f;
		}
	}
}

void Timer::ResetMyTimer()
{
	processes_.clear();
}

void Timer::Reset(std::string key)
{
	if (processes_.contains(key))
	{
		processes_.at(key).Reset();
	}
}

void Timer::Start(std::string key)
{
	if (processes_.contains(key))
	{
		processes_.at(key).isRunning_ = true;
	}
}

void Timer::Stop(std::string key)
{
	if (processes_.contains(key))
	{
		processes_.at(key).isRunning_ = false;
	}
}

void Timer::Restart(std::string key)
{
	if (processes_.contains(key))
	{
		processes_.at(key).Reset();
		processes_.at(key).isRunning_ = true;
	}
}

void Timer::Delete(string key)
{
	if (!processes_.contains(key))return;

	processes_.erase(key);
}





void Timer::SetTimer(std::string key, float maxTime, bool start)
{
	if (!processes_.contains(key))
	{
		Process process = {};
		process.isRunning_ = start;
		process.isEnd_ = false;
		if (maxTime == 0.0f) {
			// 時間を進める
			process.time_ = 0.0f;
			process.maxTime_ = maxTime;
			process.isForward_ = true;
		}
		else {
			// 時間を減らしていく
			process.time_ = maxTime;
			process.maxTime_ = maxTime;
			process.isForward_ = false;
		}
		processes_.emplace(key, process);
	}
	else
	{
		auto& process = processes_.at(key);
		process.isRunning_ = start;
		process.isEnd_ = false;
		if (maxTime == 0.0f) {
			// 時間を進める
			process.time_ = 0.0f;
			process.maxTime_ = maxTime;
			process.isForward_ = true;
		}
		else {
			// 時間を減らしていく
			process.time_ = maxTime;
			process.maxTime_ = maxTime;
			process.isForward_ = false;
		}
	}
}

void Timer::SetTime(std::string key, float time)
{
	if (processes_.contains(key))
	{
		processes_.at(key).time_ = time;
	}
}

void Timer::MultDelta(float& val)
{
	val *= deltaTime_ * whichTimeRate_;
}

float Timer::GetMultDelta(const float& val)
{
	return val * deltaTime_ * whichTimeRate_;
}

void Timer::SetWitchTimeOnMyTimer(float rate, float time)
{
	whichTimeRate_ = rate;
	whichTime_ = time;
}

float Timer::GetTime(std::string key) const
{
	if (processes_.contains(key))
	{
		return processes_.at(key).time_;
	}
	return 0.0f;
}

float Timer::GetMaxTime(std::string key) const
{
	if (processes_.contains(key))
	{
		return processes_.at(key).maxTime_;
	}
	return 0.0f;
}

float Timer::GetRate(string key) const
{
	if (processes_.contains(key))
	{
		return 1.0f - processes_.at(key).time_ / processes_.at(key).maxTime_;
	}
	return 0.0f;
}

bool Timer::IsEndTimer(std::string key) const
{
	if (processes_.contains(key))
	{
		return processes_.at(key).isEnd_;
	}
	return false;
}

bool Timer::InProgress(std::string key) const
{
	if (processes_.contains(key))
	{
		return processes_.at(key).isRunning_;
	}
	return false;
}

void Timer::RecalculationDelta()
{
	// デルタタイム
	auto nowTime = chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		chrono::duration_cast<chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
}

