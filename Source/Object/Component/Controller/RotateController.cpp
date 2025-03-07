#include "../../../Input/InputManager.h"
#include "../Common/Transform.h"
#include "RotateController.h"

RotateController::RotateController():Component(),
rotatePowMouseX_({}),
rotatePowMouseY_({})
{
}

void RotateController::UpdateComponent()
{

	auto dif = InputMng.GetMouseMovePow();

	// ‰ñ“]—Ê
	Vector3 xPow = rotatePowMouseX_ * dif.x * 0.01f;
	Vector3 yPow = rotatePowMouseY_ * dif.y * 0.01f;

	Vector3 rotatePow = xPow + yPow;

	transform_->get().qua = transform_->get().qua.Mult(
		Quaternion::Euler(rotatePow));

}

void RotateController::ReleaseComponent()
{
}
