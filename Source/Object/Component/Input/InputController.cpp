#include "../../../Input/InputManager.h"
#include "InputController.h"

InputController::InputController() : Component()
{
	inputMng_ = InputManager::GetInstance();

	isAction_.emplace(INPUT_PUSH_TYPE::PRESS,
		[&](const string& key)->bool { return inputMng_->get().Pressed(key); });
	isAction_.emplace(INPUT_PUSH_TYPE::TRRIGER,
		[&](const string& key)->bool { return inputMng_->get().Triggered(key); });
	isAction_.emplace(INPUT_PUSH_TYPE::RELEASE,
		[&](const string& key)->bool { return inputMng_->get().Released(key); });
}

void InputController::AddFunction(const string& key, INPUT_PUSH_TYPE pushType, function<void()> func)
{
	controllList_.emplace(key, pair(pushType, func));
}

void InputController::UpdateComponent()
{
	for (const auto& [key, pair] : controllList_)
	{
		const auto& [pushType, func] = pair;

		// ���͂���Ă��邩
		if (isAction_[pushType](key))
		{
			// ������s
			func();
		}
	}
}

void InputController::ReleaseComponent()
{
}
