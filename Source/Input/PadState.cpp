#include "InputInclude.h"
#include "PadState.h"

PadState::PadState(UINT padNo)
{
	currentBuffer_ = 0;
	padNo_ = padNo;
	RtlZeroMemory(state_, sizeof(XINPUT_STATE) * 2);
}

PadState::~PadState()
{
}

bool PadState::Update(void)
{
	currentBuffer_ = 1 - currentBuffer_;
	auto result = XInputGetState(padNo_, &state_[currentBuffer_]);
	if (result != ERROR_SUCCESS)
	{
		if (ERROR_DEVICE_NOT_CONNECTED == result) {
			return false;
		}
	}
	return true;
}

bool PadState::Pressed(unsigned int button, unsigned int buffer)
{
	buffer = (buffer == defaultBuffer_) ? currentBuffer_ : buffer;
	
	return (state_[buffer].Gamepad.wButtons & button);
}

bool PadState::Triggered(unsigned int button)
{
	return (Pressed(button, currentBuffer_) && !Pressed(button, 1 - currentBuffer_));
}

bool PadState::Released(unsigned int button)
{
	return (!Pressed(button, currentBuffer_) && Pressed(button, 1 - currentBuffer_));
}

int PadState::GetLeftTriggerState(unsigned int buffer)
{
	buffer = (buffer == defaultBuffer_) ? currentBuffer_ : buffer;

	return state_[buffer].Gamepad.bLeftTrigger;
}

int PadState::GetRightTriggerState(unsigned int buffer)
{
	buffer = (buffer == defaultBuffer_) ? currentBuffer_ : buffer;

	return state_[buffer].Gamepad.bRightTrigger;
}

Vector2 PadState::GetLeftStickState(unsigned int buffer)
{
	buffer = (buffer == defaultBuffer_) ? currentBuffer_ : buffer;

	Vector2 ret = {};
	ret = { static_cast<float>(state_[buffer].Gamepad.sThumbLX),
			static_cast<float>(state_[buffer].Gamepad.sThumbLY) };

	auto s = ret.Length();

	if (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE >= ret.Length())return Vector2();

	return ret;
}

Vector2 PadState::GetRightStickState(unsigned int buffer)
{
	buffer = (buffer == defaultBuffer_) ? currentBuffer_ : buffer;

	Vector2 ret = {};
	ret = { static_cast<float>(state_[buffer].Gamepad.sThumbRX),
			static_cast<float>(state_[buffer].Gamepad.sThumbRY) };

	if (XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE >= ret.Length())return Vector2();

	return ret;
}

bool PadState::AnyPressed(void)
{
	// 全ボタンをチェック
	for (unsigned int button = 1; button != 0; button <<= 1)
	{
		if (Pressed(button, currentBuffer_))
		{
			return true;
		}
	}
	return false;
}

bool PadState::AnyTriggered(void)
{
	// 全ボタンをチェック
	for (unsigned int button = 1; button != 0; button <<= 1)
	{
		if (Triggered(button))
		{
			return true;
		}
	}
	return false;

}
