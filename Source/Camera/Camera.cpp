#if defined(_DEBUG)
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_win32.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "../Input/InputManager.h"
#include "Camera.h"

namespace
{
	// カメラ初期位置
	const Position3D INIT_CAMERA_POS = { 0.0f,100.0f,-100.0f };

	// ターゲット座標への相対座標
	const Position3D TARGET_RELATIVE_POS = { 0.0f,0.0f,100.0f };

	// カメラアップベクトル初期
	const Position3D INIT_CAMERA_UPVECTOR = { 0.0f,1.0f,0.0f };

	// カメラニアファー
	const float CAMERA_NEAR = (1.0f);
	const float CAMERA_FAR = (50000.0f);

	// カメラシェイク時間管理キー
	const string CAMERA_SHAKE_TIME_KEY = "CAMERA_SHAKE";

	// カメララープ時間管理キー
	const string CAMERA_LERP_TIME_KEY = "CAMERA_LERP";

	// カメラ操作入力管理キー
	// カメラ移動
	const string INPUT_KEY_CAMERA_MOVEUP = "CAMERA_MOVEUP";
	const string INPUT_KEY_CAMERA_MOVEDOWN = "CAMERA_MOVEDOWN";
	const string INPUT_KEY_CAMERA_MOVELEFT = "CAMERA_MOVELEFT";
	const string INPUT_KEY_CAMERA_MOVERIGHT = "CAMERA_MOVERIGHT";
	const string INPUT_KEY_CAMERA_MOVEFRONT = "CAMERA_MOVEFRONT";
	const string INPUT_KEY_CAMERA_MOVEBACK = "CAMERA_MOVEBACK";
	// カメラ回転
	const string INPUT_KEY_CAMERA_ROTATERIGHT = "CAMERA_ROTATERIGHT";
	const string INPUT_KEY_CAMERA_ROTATELEFT = "CAMERA_ROTATELEFT";
	const string INPUT_KEY_CAMERA_ROTATEUP = "CAMERA_ROTATEUP";
	const string INPUT_KEY_CAMERA_ROTATEDOWN = "CAMERA_ROTATEDOWN";
}

Camera::Camera() :
	state_(STATE::FREE),
	pos_(INIT_CAMERA_POS),
	targetPos_(TARGET_RELATIVE_POS),
	up_(INIT_CAMERA_UPVECTOR),
	rot_({}),
	fov_(XM_PIDIV4),
	near_(CAMERA_NEAR),
	far_(CAMERA_FAR),
	cameraTransform_({}),
	isChanging_(false),
	frustum_({}),
	sphere_({}),
	followRelativePos_({}),
	followRate_(0.0f),
	subFollowRelativePos_({}),
	lerpStartTransform_({}),
	lerpEndTransform_({}),
	shakePow_({}),
	shakeSpeed_(0.0f),
	isPriorityShake_(false),
	viewProjHeapNo_(0),
	viewProjMap_(nullptr),
	cameraHeapNo_(0),
	cameraMap_(nullptr)
{
	// カメラ用定数バッファ作成
	CreateConstantBuffer();

	// 状態別更新処理リスト設定
	stateUpdateList_[STATE::FREE] = bind(&Camera::UpdateFree, this);
	stateUpdateList_[STATE::FOLLOW] = bind(&Camera::UpdateFollow, this);
	stateUpdateList_[STATE::RATE_FOLLOW] = bind(&Camera::UpdateRateFollow, this);
	stateUpdateList_[STATE::FIXED] = bind(&Camera::UpdateFixed, this);
	stateUpdateList_[STATE::LERP] = bind(&Camera::UpdateLerp, this);
	stateUpdateList_[STATE::LERP_FOLLOW] = bind(&Camera::UpdateLerpFollow, this);

	// STATE::FREEで初期化
	StateUpdate_ = stateUpdateList_.at(STATE::FREE);

	// 状態別更新処理リスト設定
	stateLatestUpdateList_[STATE::FREE] = bind(&Camera::LatestUpdateFree, this, placeholders::_1, placeholders::_2);
	stateLatestUpdateList_[STATE::FOLLOW] = bind(&Camera::LatestUpdateFollow, this, placeholders::_1, placeholders::_2);
	stateLatestUpdateList_[STATE::RATE_FOLLOW] = bind(&Camera::LatestUpdateRateFollow, this, placeholders::_1, placeholders::_2);
	stateLatestUpdateList_[STATE::FIXED] = bind(&Camera::LatestUpdateFixed, this, placeholders::_1, placeholders::_2);
	stateLatestUpdateList_[STATE::LERP] = bind(&Camera::LatestUpdateLerp, this, placeholders::_1, placeholders::_2);
	stateLatestUpdateList_[STATE::LERP_FOLLOW] = bind(&Camera::LatestUpdateLerpFollow, this, placeholders::_1, placeholders::_2);

	// STATE::FREEで初期化
	StateLatestUpdate_ = stateLatestUpdateList_.at(STATE::FREE);

	// 入力設定
	InputSetting();
}

Camera::~Camera()
{

}

void Camera::Terminate()
{
	viewProjMatConstBuffer_->Unmap(0, nullptr);
	cameraConstBuffer_->Unmap(0, nullptr);

	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(viewProjHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(cameraHeapNo_);
}

void Camera::ChangeState(STATE state, bool isReset)
{
	if (state_ == state)return;

	state_ = state;

	if (isReset)
	{
		ResetCamera();
	}

	if (stateUpdateList_.contains(state_))
	{
		StateUpdate_ = stateUpdateList_.at(state_);
	}

	if (stateLatestUpdateList_.contains(state_))
	{
		StateLatestUpdate_ = stateLatestUpdateList_.at(state_);
	}
}

void Camera::Update()
{
	StateUpdate_();
	
	UpdateViewProjectionMatrix();

	cameraMap_->pos = pos_;
	cameraMap_->dir = rot_.GetForward();

	cameraTransform_.PreUpdate();
	cameraTransform_.pos = pos_;
	cameraTransform_.qua = rot_;

#if defined(_DEBUG)

	ImGui::Text("CameraPos: %2.f %2.f %2.f", pos_.x, pos_.y, pos_.z);
	Vector3 r = rot_.ToEuler();
	ImGui::Text("CameraDir: %2.f %2.f %2.f", Radian2Deg(r.x), Radian2Deg(r.y), Radian2Deg(r.z));

#endif
}

void Camera::SetBeforeDraw()
{
	DxCommand->SetGraphicsRootDescriptorTable(0, HeapAllocator.GetCBV_SRV_UAVGPUAddress(viewProjHeapNo_));

	DxCommand->SetGraphicsRootDescriptorTable(2, HeapAllocator.GetCBV_SRV_UAVGPUAddress(cameraHeapNo_));
}

void Camera::SetRotate(Quaternion rot)
{
	rot_ = rot;

	// 注視点の相対座標を回転
	Position3D localRotPos = rot_.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	targetPos_ = pos_ + localRotPos;

	up_ = rot_.GetUp();
}

void Camera::InputSetting()
{
	// カメラ操作

	// 移動
	InputMng.AddKey(INPUT_KEY_CAMERA_MOVEUP, { {INPUT_TYPE::Keyboard,VK_U  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_MOVELEFT, { {INPUT_TYPE::Keyboard,VK_J  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_MOVEDOWN, { {INPUT_TYPE::Keyboard,VK_O  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_MOVERIGHT, { {INPUT_TYPE::Keyboard,VK_L  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_MOVEFRONT, { {INPUT_TYPE::Keyboard,VK_I  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_MOVEBACK, { {INPUT_TYPE::Keyboard,VK_K  } });

	// 回転
	InputMng.AddKey(INPUT_KEY_CAMERA_ROTATERIGHT, { {INPUT_TYPE::Keyboard,VK_RIGHT  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_ROTATELEFT, { {INPUT_TYPE::Keyboard,VK_LEFT  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_ROTATEUP, { {INPUT_TYPE::Keyboard,VK_UP  } });
	InputMng.AddKey(INPUT_KEY_CAMERA_ROTATEDOWN, { {INPUT_TYPE::Keyboard,VK_DOWN  } });
}

void Camera::ResetCamera()
{
	pos_ = INIT_CAMERA_POS;
	targetPos_ = TARGET_RELATIVE_POS;
	up_ = INIT_CAMERA_UPVECTOR;
	rot_ = Quaternion::Euler({ 0.0f,0.0f,0.0f });
	fov_ = XM_PIDIV4;
	near_ = CAMERA_NEAR;
	far_ = CAMERA_FAR;
	isChanging_ = true;

	follower_.reset();
	subFollower_.reset();
}

void Camera::CreateConstantBuffer()
{
	CreateConstantBufferViewProjectionMatrix();
	CreateConstantBufferCameraInfo();
}

void Camera::CreateConstantBufferViewProjectionMatrix()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(XMMATRIX) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(viewProjMatConstBuffer_.ReleaseAndGetAddressOf())
	);

	viewProjMatConstBuffer_->Map(0, nullptr, (void**)&viewProjMap_);

	XMFLOAT3 eye(pos_.x, pos_.y, pos_.z);
	XMFLOAT3 target(targetPos_.x, targetPos_.y, targetPos_.z);
	XMFLOAT3 up(up_.x, up_.y, up_.z);
	auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	auto projMat = XMMatrixPerspectiveFovLH(fov_,
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),//アス比
		near_,//近い方
		far_//遠い方
	);

	XMMATRIX mat = XMMatrixIdentity();
	mat = viewMat * projMat;

	*viewProjMap_ = mat;

	viewProjHeapNo_ = HeapAllocator.CreateCBV(viewProjMatConstBuffer_.Get());
}

void Camera::CreateConstantBufferCameraInfo()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(CameraInfo) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(cameraConstBuffer_.ReleaseAndGetAddressOf())
	);

	cameraConstBuffer_->Map(0, nullptr, (void**)&cameraMap_);

	cameraMap_->pos = pos_;
	cameraMap_->dir = rot_.GetForward();

	cameraHeapNo_ = HeapAllocator.CreateCBV(cameraConstBuffer_.Get());
}

void Camera::UpdateViewProjectionMatrix()
{
	//if (!isChanging_)return;

	XMFLOAT3 eye(pos_.x, pos_.y, pos_.z);
	XMFLOAT3 target(targetPos_.x, targetPos_.y, targetPos_.z);
	XMFLOAT3 up(up_.x, up_.y, up_.z);
	viewMatrix_ = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	projMatrix_ = XMMatrixPerspectiveFovLH(fov_,//画角は90°
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),//アス比
		near_,//近い方
		far_//遠い方
	);

	XMMATRIX mat = XMMatrixIdentity();
	mat = viewMatrix_ * projMatrix_;

	*viewProjMap_ = mat;

	isChanging_ = false;

	UpdateFrustum(mat);
}

void Camera::UpdateFrustum(const XMMATRIX& viewProj)
{
	Frustum frustum;

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, viewProj);

	// 平面を抽出（各行の係数から計算）
	frustum.planes[0] = { // Left
		{ matrix._14 + matrix._11, matrix._24 + matrix._21, matrix._34 + matrix._31 },
		matrix._44 + matrix._41
	};
	frustum.planes[1] = { // Right
		{ matrix._14 - matrix._11, matrix._24 - matrix._21, matrix._34 - matrix._31 },
		matrix._44 - matrix._41
	};
	frustum.planes[2] = { // Bottom
		{ matrix._14 + matrix._12, matrix._24 + matrix._22, matrix._34 + matrix._32 },
		matrix._44 + matrix._42
	};
	frustum.planes[3] = { // Top
		{ matrix._14 - matrix._12, matrix._24 - matrix._22, matrix._34 - matrix._32 },
		matrix._44 - matrix._42
	};
	frustum.planes[4] = { // Near
		{ matrix._13, matrix._23, matrix._33 },
		matrix._43
	};
	frustum.planes[5] = { // Far
		{ matrix._14 - matrix._13, matrix._24 - matrix._23, matrix._34 - matrix._33 },
		matrix._44 - matrix._43
	};

	// 正規化
	for (int i = 0; i < 6; ++i) {
		XMVECTOR plane = XMVectorSet(frustum.planes[i].normal.x, frustum.planes[i].normal.y, frustum.planes[i].normal.z, frustum.planes[i].d);
		plane = XMPlaneNormalize(plane);
		XMStoreFloat3(&frustum.planes[i].normal, plane);
		frustum.planes[i].d = XMVectorGetW(plane);
	}

	frustum_ = frustum;
}

void Camera::ShakeCamera(Position3D& pos)
{
	if (!MainTimer.InProgress(CAMERA_SHAKE_TIME_KEY))
	{
		isPriorityShake_ = false;
		return;
	}

	float time = (MainTimer.GetTime(CAMERA_SHAKE_TIME_KEY) * shakeSpeed_);

	float sign = sinf(time);

	auto pow = rot_.PosAxis(shakePow_);

	pos.x += pow.x * sign;
	pos.y += pow.y * sign;
	pos.z += pow.z * sign;
}

void Camera::UpdateFree()
{
	auto& input = InputManager::GetInstance();

	// 回転
	float rotPow = 1.0f;
	if (input.Pressed(INPUT_KEY_CAMERA_ROTATERIGHT))
	{
		rot_ = rot_.Mult(Quaternion::Euler(Vector3(0.0f, Deg2Radian(rotPow), 0.0f)));
	}
	else if (input.Pressed(INPUT_KEY_CAMERA_ROTATELEFT))
	{
		rot_ = rot_.Mult(Quaternion::Euler(Vector3(0.0f, Deg2Radian(-rotPow), 0.0f)));
	}
	else if (input.Pressed(INPUT_KEY_CAMERA_ROTATEUP))
	{
		rot_ = rot_.Mult(Quaternion::Euler(Vector3(Deg2Radian(-rotPow), 0.0f, 0.0f)));
	}
	else if (input.Pressed(INPUT_KEY_CAMERA_ROTATEDOWN))
	{
		rot_ = rot_.Mult(Quaternion::Euler(Vector3(Deg2Radian(rotPow), 0.0f, 0.0f)));
	}

	// 移動
	float movePow = 5.0f;
	if (input.Pressed(INPUT_KEY_CAMERA_MOVEFRONT))
	{
		pos_ += Quaternion::ToEuler(rot_.GetForward()) * movePow;
	}
	if (input.Pressed(INPUT_KEY_CAMERA_MOVELEFT))
	{
		pos_ += Quaternion::ToEuler(rot_.GetLeft()) * movePow;
	}
	if (input.Pressed(INPUT_KEY_CAMERA_MOVERIGHT))
	{
		pos_ += Vector3(rot_.GetRight()) * movePow;
	}
	if (input.Pressed(INPUT_KEY_CAMERA_MOVEBACK))
	{
		pos_ += Quaternion::ToEuler(rot_.GetBack()) * movePow;
	}

	// カメラ上方向
	if (input.Pressed(INPUT_KEY_CAMERA_MOVEUP)) { pos_.y += movePow; }
	// カメラ下方向
	if (input.Pressed(INPUT_KEY_CAMERA_MOVEDOWN)) { pos_.y -= movePow; }


	// 注視点の相対座標を回転
	Position3D localRotPos = rot_.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	targetPos_ = pos_ + localRotPos;

	// カメラの上方向
	up_ = rot_.GetUp();

	isChanging_ = true;

}

void Camera::UpdateFollow()
{
	if (!follower_.has_value())return;

	const auto& fPos = follower_->get().pos;

	rot_ = follower_->get().qua;

	// 注視点の相対座標を回転
	Position3D localRotPos = rot_.PosAxis(followRelativePos_);

	pos_ = fPos + localRotPos;


	ShakeCamera(pos_);

	// 注視点の相対座標を回転
	localRotPos = rot_.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	targetPos_ = pos_ + localRotPos;

	// カメラの上方向
	up_ = rot_.GetUp();

	isChanging_ = true;
}

void Camera::UpdateRateFollow()
{
	if (!follower_.has_value() || !subFollower_.has_value())return;

	const auto& fPos = follower_->get().pos;

	auto mainRot = follower_->get().qua;

	// 注視点の相対座標を回転
	Position3D localRotPos = mainRot.PosAxis(followRelativePos_);

	auto mainPos = fPos + localRotPos;


	ShakeCamera(mainPos);

	// 注視点の相対座標を回転
	localRotPos = mainRot.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	auto mainTargetPos = mainPos + localRotPos;

	const auto& subFPos = subFollower_->get().pos;

	auto subRot = subFollower_->get().qua;

	// 注視点の相対座標を回転
	localRotPos = subRot.PosAxis(subFollowRelativePos_);

	auto subPos = subFPos + localRotPos;


	ShakeCamera(subPos);

	// 注視点の相対座標を回転
	localRotPos = subRot.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	auto subTargetPos = subPos + localRotPos;


	rot_ = Quaternion::Slerp(mainRot, subRot, static_cast<float>(followRate_));

	pos_ = Lerp(mainPos, subPos, followRate_);
	targetPos_ = Lerp(mainTargetPos, subTargetPos, followRate_);

	// カメラの上方向
	up_ = rot_.GetUp();

	isChanging_ = true;
}

void Camera::UpdateFixed()
{
	isChanging_ = true;
}

void Camera::UpdateLerp()
{
	float rate = 1.0f - MainTimer.GetTime(CAMERA_LERP_TIME_KEY) / MainTimer.GetMaxTime(CAMERA_LERP_TIME_KEY);

	Position3D lerpPos = Lerp(
		lerpStartTransform_.pos, lerpEndTransform_.pos, rate
	);
	Quaternion lerpRot =
		Quaternion::Slerp(lerpStartTransform_.qua, lerpEndTransform_.qua, rate);

	const auto& fPos = lerpPos;

	rot_ = lerpRot;

	// 注視点の相対座標を回転
	Position3D localRotPos = rot_.PosAxis({});

	pos_ = fPos + localRotPos;


	ShakeCamera(pos_);

	// 注視点の相対座標を回転
	localRotPos = rot_.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	targetPos_ = pos_ + localRotPos;

	// カメラの上方向
	up_ = rot_.GetUp();

	isChanging_ = true;

}

void Camera::UpdateLerpFollow()
{
	float rate = 1.0f - MainTimer.GetTime(CAMERA_LERP_TIME_KEY) / MainTimer.GetMaxTime(CAMERA_LERP_TIME_KEY);

	Position3D lerpPos = Lerp(
		lerpStartTransform_.pos, lerpEndFollower_->get().pos, rate
	);
	Quaternion lerpRot =
		Quaternion::Slerp(lerpStartTransform_.qua, lerpEndFollower_->get().qua, rate);

	const auto& fPos = lerpPos;

	rot_ = lerpRot;

	// 注視点の相対座標を回転
	Position3D localRotPos = rot_.PosAxis(followRelativePos_);

	pos_ = fPos + localRotPos;


	ShakeCamera(pos_);

	// 注視点の相対座標を回転
	localRotPos = rot_.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	targetPos_ = pos_ + localRotPos;

	// カメラの上方向
	up_ = rot_.GetUp();

	isChanging_ = true;
}

void Camera::LatestUpdateFree(Position3D& cameraPos, Position3D& targetPos)const
{
}

void Camera::LatestUpdateFollow(Position3D& cameraPos, Position3D& targetPos)const
{
}

void Camera::LatestUpdateRateFollow(Position3D& cameraPos, Position3D& targetPos)const
{
	if (!follower_.has_value() || !subFollower_.has_value())return;

	const auto& fPos = follower_->get().pos;

	auto mainRot = follower_->get().qua;

	// 注視点の相対座標を回転
	Position3D localRotPos = mainRot.PosAxis(followRelativePos_);

	auto mainPos = fPos + localRotPos;

	// 注視点の相対座標を回転
	localRotPos = mainRot.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	auto mainTargetPos = mainPos + localRotPos;



	const auto& subFPos = subFollower_->get().pos;

	auto subRot = subFollower_->get().qua;

	// 注視点の相対座標を回転
	localRotPos = subRot.PosAxis(subFollowRelativePos_);

	auto subPos = subFPos + localRotPos;

	// 注視点の相対座標を回転
	localRotPos = subRot.PosAxis(TARGET_RELATIVE_POS);

	// 注視点をワールド座標に変換
	auto subTargetPos = subPos + localRotPos;


	//rot_ = Quaternion::Slerp(mainRot, subRot, static_cast<float>(followRate_));

	cameraPos = Lerp(mainPos, subPos, followRate_);
	targetPos = Lerp(mainTargetPos, subTargetPos, followRate_);

	//// カメラの上方向
	//up_ = rot_.GetUp();

	//isChanging_ = true;
}

void Camera::LatestUpdateFixed(Position3D& cameraPos, Position3D& targetPos)const
{
}

void Camera::LatestUpdateLerp(Position3D& cameraPos, Position3D& targetPos)const
{

}

void Camera::LatestUpdateLerpFollow(Position3D& cameraPos, Position3D& targetPos) const
{
}

void Camera::SetLerpConfig(const Transform& before, const Transform& after, float time)
{
	MainTimer.Delete(CAMERA_LERP_TIME_KEY);

	MainTimer.SetTimer(CAMERA_LERP_TIME_KEY, time,true);

	lerpStartTransform_ = before;
	lerpEndTransform_ = after;
}

void Camera::SetLerpConfig(const Transform& after, float time)
{
	MainTimer.Delete(CAMERA_LERP_TIME_KEY);

	MainTimer.SetTimer(CAMERA_LERP_TIME_KEY, time, true);

	lerpStartTransform_.pos = pos_;
	lerpStartTransform_.qua = rot_;
	lerpEndTransform_ = after;
}

void Camera::SetLerpFollowConfig(const Transform& after, float time)
{
	MainTimer.Delete(CAMERA_LERP_TIME_KEY);

	MainTimer.SetTimer(CAMERA_LERP_TIME_KEY, time, true);

	lerpStartTransform_.pos = pos_;
	lerpStartTransform_.qua = rot_;
	lerpEndFollower_ = after;
}

void Camera::Shake(float time, Vector3 pow, float speed, bool isPriority)
{
	if (isPriorityShake_)return;

	isPriorityShake_ = isPriority;

	MainTimer.SetTimer(CAMERA_SHAKE_TIME_KEY, time, true);

	shakePow_ = pow;
	shakeSpeed_ = speed;
}

Position3D Camera::GetLatestPos() const
{
	Position3D pos = {};
	Position3D targetPos = {};

	LatestUpdateRateFollow(pos, targetPos);
	//StateLatestUpdate_(pos, targetPos);

	return pos;
}

Position3D Camera::GetLatestTargetPos() const
{
	Position3D pos = {};
	Position3D targetPos = {};

	LatestUpdateRateFollow(pos, targetPos);
	//StateLatestUpdate_(pos, targetPos);

	return targetPos;
}

bool Camera::IsEndLerp() const
{
	return !MainTimer.InProgress(CAMERA_LERP_TIME_KEY);
}
