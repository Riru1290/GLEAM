#pragma once
class ClearChecker
{
public:

	ClearChecker();
	~ClearChecker() = default;

	[[nodiscard]] const bool& IsClear()const { return isClear_; }

private:

	bool isClear_;
};

