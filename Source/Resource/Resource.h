#pragma once

/// @brief リソース基底クラス
class Resource
{
public:

	Resource() = default;
	virtual ~Resource() = default;

	/// @brief 解放処理
	virtual void Release() = 0;
};

