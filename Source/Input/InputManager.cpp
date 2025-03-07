#include "InputInclude.h"
#include "../Application/Application.h"
#include "KeyboardState.h"
#include "PadState.h"
#include "InputManager.h"

InputManager::InputManager()
{
	keyboard_ = make_unique<KeyboardState>(Application::GetInstance().GetWindow().GetHWND());
	pad_[PAD_1] = make_unique<PadState>(PAD_1);
	pad_[PAD_2] = make_unique<PadState>(PAD_2);
	pad_[PAD_3] = make_unique<PadState>(PAD_3);
	pad_[PAD_4] = make_unique<PadState>(PAD_4);
}

InputManager::~InputManager()
{
}

void InputManager::MouseDataUpdate()
{
	// �E�B���h�E�n���h��
	HWND hWnd = Application::GetInstance().GetWindow().GetHWND();

	// 1�t���[���O�}�E�X���W�X�V
	preMousePos_ = mousePos_;

	// ��ʑS�̂ł̃}�E�X�ʒu���擾
	POINT mousePos;
	GetCursorPos(&mousePos);

	// �N���C�A���g���W�ɕϊ�
	ScreenToClient(hWnd, &mousePos);

	// �E�B���h�E�T�C�Y���擾
	LPRECT wRect = new RECT;
	GetWindowRect(Application::GetInstance().GetWindow().GetHWND(), wRect);

	// RECT��Size2D�ɕϊ�
	const Size2D& displaySize = { static_cast<float>(wRect->right - wRect->left),static_cast<float>(wRect->bottom - wRect->top) };

	// �f�B�X�v���C�𑜓x�ƃE�B���h�E�T�C�Y�̔䗦���v�Z
	Size2D displayRate = { static_cast<float>(SCREEN_WIDTH) / displaySize.x,static_cast<float>(SCREEN_HEIGHT) / displaySize.y };

	// �䗦�����Ƀ}�E�X���W���v�Z
	mousePos_ = Position2D(static_cast<float>(mousePos.x) * displayRate.x, static_cast<float>(mousePos.y) * displayRate.y);

	// �}�E�X�ړ��ʌv�Z
	mouseMovePow_ = mousePos_ - preMousePos_;

	// �Œ�
	if (isFixedMouse_)
	{
		// �E�B���h�E�̒��S���v�Z
		POINT screenCenter;
		screenCenter.x = static_cast<int>(SCREEN_CENTER.x / displayRate.x);
		screenCenter.y = static_cast<int>(SCREEN_CENTER.y / displayRate.y);

		// �X�N���[�����W�ɕϊ�
		ClientToScreen(hWnd, &screenCenter);

		// POINT��Position2D�ɕϊ�
		Position2D center = { static_cast<float>(screenCenter.x),static_cast<float>(screenCenter.y)};

		// �J�[�\�����Œ�
		SetCursorPos(static_cast<int>(center.x), static_cast<int>(center.y));

		// �}�E�X�ړ��ʌv�Z
		// �䗦�����Ƀ}�E�X���W���v�Z
		POINT mouse;
		mouse.x = static_cast<int>(mousePos_.x / displayRate.x);
		mouse.y = static_cast<int>(mousePos_.y / displayRate.y);

		// �X�N���[�����W�ɕϊ�
		ClientToScreen(hWnd, &mouse);

		// POINT��Position2D�ɕϊ�
		Position2D m = { static_cast<float>(mouse.x),static_cast<float>(mouse.y) };

		// �������v�Z
		auto dif = m - center;
		if (dif.Length() > 1.0f) {
			mouseMovePow_ = m - center;
		}
		else {
			mouseMovePow_ = {};
		}
	}

	// �ꎞ�ϐ����
	delete wRect;
}

void InputManager::Terminate()
{
	keyConfig_.clear();
}

void InputManager::Update()
{
	// �L�[�{�[�h���͍X�V
	keyboard_->Update();

	// �p�b�h���͍X�V
	connectingPadNum_ = 0;
	for (auto& pad : pad_) {
		if (pad->Update()) {
			connectingPadNum_++;
		}
	}

	// �}�E�X���͍X�V
	MouseDataUpdate();
}

void InputManager::AddKey(const string& key, vector<INPUT_DATA> list)
{
	if (keyConfig_.contains(key))
	{
		for (const auto& l : list)
		{
			keyConfig_.at(key).emplace_back(l);
		}
	}
	else
	{
		keyConfig_.emplace(key, list);
		keyHoldTime_.emplace(key, 0.0f);
	}
}

bool InputManager::Pressed(const string& key, UINT padNo)
{
	if (!keyConfig_.contains(key))
	{
		return false;
	}

	bool rtn = false;
	for (const auto& k : keyConfig_.at(key))
	{
		switch (k.type_)
		{
		case INPUT_TYPE::Keyboard:
			if (padNo != UINT_MAX)continue;
			rtn |= keyboard_->Pressed(k.key_);
			break;
		case INPUT_TYPE::Pad:
			if (padNo == UINT_MAX) {
				padNo = PAD_1;
			}
			rtn |= pad_[padNo]->Pressed(k.key_);
			break;
		default:
			break;
		}
	}
	return rtn;
}

bool InputManager::Triggered(const string& key, UINT padNo)
{
	if (!keyConfig_.contains(key))
	{
		return false;
	}

	bool rtn = false;
	for (const auto& k : keyConfig_.at(key))
	{
		switch (k.type_)
		{
		case INPUT_TYPE::Keyboard:
			if (padNo != UINT_MAX)continue;
			rtn |= keyboard_->Triggered(k.key_);
			break;
		case INPUT_TYPE::Pad:
			if (padNo == UINT_MAX) {
				padNo = PAD_1;
			}
			rtn |= pad_[padNo]->Triggered(k.key_);
			break;
		default:
			break;
		}
	}
	return rtn;
}

bool InputManager::Released(const std::string& key, UINT padNo)
{
	if (!keyConfig_.contains(key))
	{
		return false;
	}

	bool rtn = false;
	for (const auto& k : keyConfig_.at(key))
	{
		switch (k.type_)
		{
		case INPUT_TYPE::Keyboard:
			if (padNo != UINT_MAX)continue;
			rtn |= keyboard_->Released(k.key_);
			break;
		case INPUT_TYPE::Pad:
			if (padNo == UINT_MAX) {
				padNo = PAD_1;
			}
			rtn |= pad_[padNo]->Released(k.key_);
			break;
		default:
			break;
		}
	}
	return rtn;
}

bool InputManager::Hold(float time, const string& key, UINT padNo)
{
	if (!keyConfig_.contains(key))
	{
		return false;
	}

	if (Pressed(key, padNo))
	{
		keyHoldTime_[key] += MainTimer.GetDelta();
	}
	else
	{
		keyHoldTime_[key] = 0.0f;
	}

	return (keyHoldTime_[key] >= time);
}

int InputManager::GetLeftTriggerState(UINT padNo)
{
	return pad_[padNo]->GetLeftTriggerState();
}

int InputManager::GetRightTriggerState(UINT padNo)
{
	return pad_[padNo]->GetRightTriggerState();
}

Vector2 InputManager::GetLeftStickState(UINT padNo)
{
	return pad_[padNo]->GetLeftStickState();
}

Vector2 InputManager::GetRightStickState(UINT padNo)
{
	return pad_[padNo]->GetRightStickState();
}

bool InputManager::AnyPressed(UINT padNo)
{
	if (padNo == UINT_MAX) 
	{
		return keyboard_->AnyPressed();
	}
	else
	{
		return pad_[padNo]->AnyPressed();
	}
	return false;
}

bool InputManager::AnyTriggered(UINT padNo)
{
	if (padNo == UINT_MAX)
	{
		return keyboard_->AnyTriggered();
	}
	else
	{
		return pad_[padNo]->AnyTriggered();
	}
	return false;
}

vector<INPUT_DATA> InputManager::GetKeyConfig(const string& key)
{
	vector<INPUT_DATA> temp;
	if (!keyConfig_.contains(key))return temp;

	return keyConfig_.at(key);
}

void InputManager::SetMouseMovePowFromWindow(Vector2 mouseMovePow)
{
	mouseMovePow_ = mouseMovePow;
}
