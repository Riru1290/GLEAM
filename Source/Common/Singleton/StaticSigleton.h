#pragma once

// 
#define PROOF_OF_STATIC_SINGLETON(TYPE)\
TYPE(const TYPE&) = delete;\
TYPE& operator=(const TYPE&) = delete;\
friend class StaticSingleton<TYPE>;

/// @brief スタティックシングルトンテンプレート
template<class T>
class StaticSingleton
{
public:

	/// @brief 静的インスタンス取得
	/// @return 静的インスタンス
	[[nodiscard]] static T& GetInstance()
	{
		static T instance;
		return instance;
	}

	/// @brief 解放処理
	virtual void Terminate() = 0;

protected:

	StaticSingleton() = default;
	virtual ~StaticSingleton() = default;
	StaticSingleton(const StaticSingleton&) = delete;
	StaticSingleton& operator=(const StaticSingleton&) = delete;

};
