#if defined(_DEBUG)
#include "../../External/imgui/imgui.h"
#include "../../External/imgui/imgui_impl_win32.h"
#include "../../External/imgui/imgui_impl_dx12.h"
#endif
#include "../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "Camera2D.h"

namespace
{
	const Position2D INIT_CAMERA_POS = { 0.0f,0.0f };
	const float INIT_ANGLE = 0.0f;
	const float INIT_LENS = 1.0f;
}

Camera2D::Camera2D() :
	state_(STATE::FREE),
	pos_(INIT_CAMERA_POS),
	angle_(INIT_ANGLE),
	lens_(INIT_LENS),
	isChanging_(false),
	cameraHeapNo_(0),
	cameraMap_(nullptr)
{
	CreateConstantBufferScreenMatrix();

	stateUpdateList_[STATE::FREE] = bind(&Camera2D::UpdateFree, this);
	stateUpdateList_[STATE::FIXED] = bind(&Camera2D::UpdateFixed, this);

	StateUpdate_ = stateUpdateList_.at(STATE::FREE);
}

Camera2D::~Camera2D()
{
}

void Camera2D::Terminate()
{
	cameraConstBuffer_->Unmap(0, nullptr);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(cameraHeapNo_);
}

void Camera2D::ChangeState(STATE state)
{
	if (state_ == state)return;

	state_ = state;

	if (stateUpdateList_.contains(state_))
	{
		StateUpdate_ = stateUpdateList_.at(state_);
	}
}

void Camera2D::Update()
{
	StateUpdate_();

	UpdateScreenMatrix();

#if defined(_DEBUG)

	ImGui::Text("2DCameraPos: %2.f %2.f", pos_.x, pos_.y);

#endif
}

void Camera2D::SetBeforeDraw()
{
	DxCommand->SetGraphicsRootDescriptorTable(0, HeapAllocator.GetCBV_SRV_UAVGPUAddress(cameraHeapNo_));
}

void Camera2D::UpdateFree()
{
}

void Camera2D::UpdateFixed()
{
}

void Camera2D::CreateConstantBufferScreenMatrix()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(Camera2DInfo) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(cameraConstBuffer_.ReleaseAndGetAddressOf())
	);

	cameraConstBuffer_->Map(0, nullptr, (void**)&cameraMap_);

	// スクリーン座標系変換
	XMMATRIX mat = XMMatrixIdentity();
	mat.r[0].m128_f32[0] = 2.0f / static_cast<float>(SCREEN_WIDTH);
	mat.r[1].m128_f32[1] = -2.0f / static_cast<float>(SCREEN_HEIGHT);
	mat.r[3].m128_f32[0] = -1.0f;
	mat.r[3].m128_f32[1] = 1.0f;

	cameraMap_->pos = pos_;
	cameraMap_->angle = angle_;
	cameraMap_->lens = lens_;
	cameraMap_->screenMat = mat;

	cameraHeapNo_ = HeapAllocator.CreateCBV(cameraConstBuffer_.Get());
}

void Camera2D::UpdateScreenMatrix()
{
}
