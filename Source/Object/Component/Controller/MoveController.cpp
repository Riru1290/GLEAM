#include "../../../Input/InputManager.h"
#include "../Common/Transform.h"
#include "MoveController.h"

namespace
{
	const string MOVE_LEFT_KEY = "MoveLeft";
	const string MOVE_RIGHT_KEY = "MoveRight";
	const string MOVE_FORWARD_KEY = "MoveForward";
	const string MOVE_BACK_KEY = "MoveBack";
	const string MOVE_RUN_KEY = "Run";
}

MoveController::MoveController()
{
	InputMng.AddKey(MOVE_LEFT_KEY, { {INPUT_TYPE::Keyboard,VK_A} });
	InputMng.AddKey(MOVE_RIGHT_KEY, { {INPUT_TYPE::Keyboard,VK_D} });
	InputMng.AddKey(MOVE_FORWARD_KEY, { {INPUT_TYPE::Keyboard,VK_W} });
	InputMng.AddKey(MOVE_BACK_KEY, { {INPUT_TYPE::Keyboard,VK_S} });
	InputMng.AddKey(MOVE_RUN_KEY, { {INPUT_TYPE::Keyboard,VK_LSHIFT} });
}

void MoveController::UpdateComponent()
{
	Vector3 movePow = {};
	Vector3 moveDir = {};

	if (InputMng.Pressed(MOVE_LEFT_KEY))
	{
		moveDir += Vector3(-1.0f, 0.0f, 0.0f);
	}
	else if (InputMng.Pressed(MOVE_RIGHT_KEY))
	{
		moveDir += Vector3(1.0f, 0.0f, 0.0f);
	}

	if (InputMng.Pressed(MOVE_FORWARD_KEY))
	{
		moveDir += Vector3(0.0f, 0.0f, 1.0f);
	}
	else if (InputMng.Pressed(MOVE_BACK_KEY))
	{
		moveDir += Vector3(0.0f, 0.0f, -1.0f);
	}

	if (moveDir != Vector3(0.0f, 0.0f, 0.0f))
	{
		float speed = walkSpeed_;
		if (InputMng.Pressed(MOVE_RUN_KEY))
		{
			speed = runSpeed_;

			RunAction_();
		}
		else
		{
			WalkAction_();
		}

		movePow = moveDir.Normalized() * MainTimer.GetMultDelta(speed);

		transform_->get().pos +=
			transform_->get().qua.PosAxis(movePow);
	}
	else
	{
		IdleAction_();
	}
}

void MoveController::ReleaseComponent()
{
}
