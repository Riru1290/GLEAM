#pragma once

/// @brief ���\�[�X���N���X
class Resource
{
public:

	Resource() = default;
	virtual ~Resource() = default;

	/// @brief �������
	virtual void Release() = 0;
};

