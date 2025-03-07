#include "KeyboardState.h"

KeyboardState::KeyboardState(const HWND& hWnd)
{
	currentBuffer_ = 0;
	for (int i = 0; i < 256; i++) {
		state_[0][i] = 0;
		state_[1][i] = 0x80;
	}
}

KeyboardState::~KeyboardState()
{
}

void KeyboardState::Update(void)
{
	currentBuffer_ = 1 - currentBuffer_;
	if (!GetKeyboardState(state_[currentBuffer_]))
	{
		assert(0 && "Keyboard");
	}


}

bool KeyboardState::Pressed(unsigned int key, unsigned int buffer)
{
	buffer = (buffer == defaultBuffer_) ? currentBuffer_ : buffer;

	return (state_[buffer][key] & 0x80);
}

bool KeyboardState::Triggered(unsigned int key)
{
	return (Pressed(key,currentBuffer_) && !Pressed(key,1 - currentBuffer_));
}

bool KeyboardState::Released(unsigned int key)
{
	return (!Pressed(key, currentBuffer_) && Pressed(key, 1 - currentBuffer_));
}

bool KeyboardState::AnyPressed(void)
{
	for (int key = 0; key < 256; ++key)
	{
		if (Pressed(key, currentBuffer_))
		{
			return true;
		}
	}
	return false;
}

bool KeyboardState::AnyTriggered(void)
{
	for (int key = 0; key < 256; ++key)
	{
		if (Triggered(key))
		{
			return true;
		}
	}
	return false;
}
