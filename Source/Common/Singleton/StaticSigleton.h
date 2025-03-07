#pragma once

// 
#define PROOF_OF_STATIC_SINGLETON(TYPE)\
TYPE(const TYPE&) = delete;\
TYPE& operator=(const TYPE&) = delete;\
friend class StaticSingleton<TYPE>;

/// @brief �X�^�e�B�b�N�V���O���g���e���v���[�g
template<class T>
class StaticSingleton
{
public:

	/// @brief �ÓI�C���X�^���X�擾
	/// @return �ÓI�C���X�^���X
	[[nodiscard]] static T& GetInstance()
	{
		static T instance;
		return instance;
	}

	/// @brief �������
	virtual void Terminate() = 0;

protected:

	StaticSingleton() = default;
	virtual ~StaticSingleton() = default;
	StaticSingleton(const StaticSingleton&) = delete;
	StaticSingleton& operator=(const StaticSingleton&) = delete;

};
