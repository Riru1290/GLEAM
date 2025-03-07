#include "../../../Input/InputManager.h"
#include "../Renderer/Renderers/Single/Model/AnimationModelRenderer.h"
#include "RotateBoneController.h"

RotateBoneController::RotateBoneController() :Component(),
rotatePowMouseX_({}),
rotatePowMouseY_({})
{
}





void RotateBoneController::UpdateComponent()
{
	auto dif = InputMng.GetMouseMovePow();

	// ‰ñ“]—Ê
	Vector3 xPow = rotatePowMouseX_ * dif.x * 0.01f;
	Vector3 yPow = rotatePowMouseY_ * dif.y * 0.01f;

	Vector3 rotatePow = xPow + yPow;

	XMMATRIX bmat = modelRenderer_->get().GetLocalAnimBoneMatrix(boneNo_);

	auto t = GetTranslationFromMatrix(bmat);


	XMMATRIX m = XMMatrixIdentity();

	auto angle = boneRot_.ToEuler();

	angle += rotatePow;

	if (isLimitRotateX_)
	{
		angle.x = min(angle.x, rotateMax_.x);
		angle.x = max(angle.x, rotateMin_.x);
	}

	if (isLimitRotateY_)
	{
		angle.y = min(angle.y, rotateMax_.y);
		angle.y = max(angle.y, rotateMin_.y);
	}

	if (isLimitRotateZ_)
	{
		angle.z = min(angle.z, rotateMax_.z);
		angle.z = max(angle.z, rotateMin_.z);
	}

	boneRot_ = Quaternion::Euler(angle);

	m *= XMMatrixTranslation(-t.x, -t.y, -t.z);
	m *= Quaternion::QuaternionToMatrix(boneRot_);
	m *= XMMatrixTranslation(t.x, t.y, t.z);

	modelRenderer_->get().MultiplyMatrixToModelBoneAndChildren(m, boneNo_);
}

void RotateBoneController::ReleaseComponent()
{
}

void RotateBoneController::ResetRotate()
{
	XMMATRIX bmat = modelRenderer_->get().GetLocalAnimBoneMatrix(boneNo_);

	auto t = GetTranslationFromMatrix(bmat);


	XMMATRIX m = XMMatrixIdentity();

	Vector3 angle = {0.0f,0.0f,0.0f};

	boneRot_ = Quaternion::Euler(angle);

	m *= XMMatrixTranslation(-t.x, -t.y, -t.z);
	m *= Quaternion::QuaternionToMatrix(boneRot_);
	m *= XMMatrixTranslation(t.x, t.y, t.z);

	modelRenderer_->get().MultiplyMatrixToModelBoneAndChildren(m, boneNo_);
}

void RotateBoneController::SetRotate(Quaternion rotate)
{


	XMMATRIX bmat = modelRenderer_->get().GetLocalAnimBoneMatrix(boneNo_);

	auto t = GetTranslationFromMatrix(bmat);


	XMMATRIX m = XMMatrixIdentity();

	auto angle = rotate.ToEuler();

	if (isLimitRotateX_)
	{
		angle.x = min(angle.x, rotateMax_.x);
		angle.x = max(angle.x, rotateMin_.x);
	}

	if (isLimitRotateY_)
	{
		angle.y = min(angle.y, rotateMax_.y);
		angle.y = max(angle.y, rotateMin_.y);
	}

	if (isLimitRotateZ_)
	{
		angle.z = min(angle.z, rotateMax_.z);
		angle.z = max(angle.z, rotateMin_.z);
	}

	boneRot_ = Quaternion::Euler(angle);

	m *= XMMatrixTranslation(-t.x, -t.y, -t.z);
	m *= Quaternion::QuaternionToMatrix(boneRot_);
	m *= XMMatrixTranslation(t.x, t.y, t.z);

	modelRenderer_->get().MultiplyMatrixToModelBoneAndChildren(m, boneNo_);
}
