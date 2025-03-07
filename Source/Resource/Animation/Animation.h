#pragma once
#include "../Resource.h"
#include "../Model/ModelData.h"

class Animation final :public Resource
{
public:

	[[nodiscard]] Animation(const string& filePath);
	~Animation() = default;

	[[nodiscard]] const ANIMATION_DATA& GetData()const { return data_; }

private:

	/// @brief ‰ğ•úˆ—
	void Release()override;

	ANIMATION_DATA data_;

};

