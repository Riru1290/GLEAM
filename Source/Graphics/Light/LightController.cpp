#ifdef _DEBUG
#include "../DebugRender/DebugRenderManager.h"
#endif
#include "../../Camera/Camera.h"
#include "../../Utility/Math/Quaternion.h"
#include "../../DirectX/DescriptorHeap/DescriptorHeapAllocator.h"
#include "LightController.h"

namespace
{
	const int HEMISPHERE_LIGHT_MAX_NUM	= (1);
	const int DIRECTIONAL_LIGHT_MAX_NUM = (3);
	const int POINT_LIGHT_MAX_NUM		= (30);
	const int SPOT_LIGHT_MAX_NUM		= (30);
}


LightController::LightController()
{
	CreateConstantBufferLightNum();
	CreateConstantBufferHemisphereLight();
	CreateConstantBufferDirectionalLight();
	CreateConstantBufferPointLight();
	CreateConstantBufferSpotLight();
}

LightController::~LightController()
{
}

void LightController::CreateConstantBufferLightNum()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(LIGHT_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(lightNumConstBuffer_.ReleaseAndGetAddressOf())
	);

	lightNumConstBuffer_->Map(0, nullptr, (void**)&lightNumMap_);

	lightNumHeapNo_ = HeapAllocator.CreateCBV(lightNumConstBuffer_.Get());

	LIGHT_NUM lightNum = {};
	*lightNumMap_ = lightNum;
}

void LightController::CreateConstantBufferHemisphereLight()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(HEMISPHERE_LIGHT) * HEMISPHERE_LIGHT_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(hemisphereLightShaderResource_.ReleaseAndGetAddressOf())
	);

	hemisphereLightShaderResource_->Map(0, nullptr, (void**)&hemisphereLightMap_);

	// シェーダリソースビュー（SRV）を作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = HEMISPHERE_LIGHT_MAX_NUM;
	srvDesc.Buffer.StructureByteStride = sizeof(HEMISPHERE_LIGHT);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	hemisphereLightHeapNo_ = HeapAllocator.CreateSRV(hemisphereLightShaderResource_.Get(), srvDesc);
}

void LightController::CreateConstantBufferDirectionalLight()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(DIRECTIONAL_LIGHT) * DIRECTIONAL_LIGHT_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(directionalLightShaderResource_.ReleaseAndGetAddressOf())
	);

	directionalLightShaderResource_->Map(0, nullptr, (void**)&directionalLightMap_);

	// シェーダリソースビュー（SRV）を作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = DIRECTIONAL_LIGHT_MAX_NUM;
	srvDesc.Buffer.StructureByteStride = sizeof(DIRECTIONAL_LIGHT);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	directionalLightHeapNo_ = HeapAllocator.CreateSRV(directionalLightShaderResource_.Get(), srvDesc);
}

void LightController::CreateConstantBufferPointLight()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(POINT_LIGHT) * POINT_LIGHT_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(pointLightShaderResource_.ReleaseAndGetAddressOf())
	);

	pointLightShaderResource_->Map(0, nullptr, (void**)&pointLightMap_);

	// シェーダリソースビュー（SRV）を作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = POINT_LIGHT_MAX_NUM;
	srvDesc.Buffer.StructureByteStride = sizeof(POINT_LIGHT);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	pointLightHeapNo_ = HeapAllocator.CreateSRV(pointLightShaderResource_.Get(), srvDesc);
}

void LightController::CreateConstantBufferSpotLight()
{
	auto heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(((sizeof(SPOT_LIGHT) * SPOT_LIGHT_MAX_NUM) + 0xff) & ~0xff);

	MainDevice->CreateCommittedResource(
		&heapP,
		D3D12_HEAP_FLAG_NONE,
		&heapDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(spotLightShaderResource_.ReleaseAndGetAddressOf())
	);

	spotLightShaderResource_->Map(0, nullptr, (void**)&spotLightMap_);

	// シェーダリソースビュー（SRV）を作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = SPOT_LIGHT_MAX_NUM;
	srvDesc.Buffer.StructureByteStride = sizeof(SPOT_LIGHT);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	spotLightHeapNo_ = HeapAllocator.CreateSRV(spotLightShaderResource_.Get(), srvDesc);
}



void LightController::Terminate()
{
	hemisphereLights_.clear();
	directionalLights_.clear();
	pointLights_.clear();
	spotLights_.clear();

	lightNumConstBuffer_->Unmap(0, nullptr);
	hemisphereLightShaderResource_->Unmap(0, nullptr);
	directionalLightShaderResource_->Unmap(0, nullptr);
	pointLightShaderResource_->Unmap(0, nullptr);
	spotLightShaderResource_->Unmap(0, nullptr);


	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(lightNumHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(hemisphereLightHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(directionalLightHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(pointLightHeapNo_);
	HeapAllocator.ReleaseDescriptorCBV_SRV_UAV(spotLightHeapNo_);
}

void LightController::Update()
{
	// 現在使用しているライト行列数リセット
	useLightCameraNum_ = 0;

	// 半球ライトカメラ行列を計算
	CalculateHemisphereLightViewProjectionMatrix();

	// 指向性ライトカメラ行列を計算
	CalculateDirectionalLightViewProjectionMatrix();

	// ポイントライトカメラ行列を計算
	CalculatePointLightViewProjectionMatrix();

	// スポットライトカメラ行列を計算
	CalculateSpotLightViewProjectionMatrix();

	// ライトの数
	lightNumMap_->hemisphereLightNum = static_cast<UINT>(hemisphereLights_.size());
	lightNumMap_->directionalLightNum = static_cast<UINT>(directionalLights_.size());
	lightNumMap_->pointLightNum = static_cast<UINT>(pointLights_.size());
	lightNumMap_->spotLightNum = static_cast<UINT>(spotLights_.size());
	lightNumMap_->useLightCameraNum = useLightCameraNum_;


	// 半球ライト
	for (int i = 0; const auto & [key, light] : hemisphereLights_)
	{
		hemisphereLightMap_[i] = *light;
		i++;
	}

	// 指向性ライト
	for (int i = 0; const auto& [key,light] : directionalLights_)
	{
		directionalLightMap_[i] = *light;
		i++;
	}

	// ポイントライト
	for (int i = 0; const auto & [key, light] : pointLights_)
	{
		pointLightMap_[i] = *light;
		i++;
	}

	// スポットライト
	for (int i = 0; const auto & [key, light] : spotLights_)
	{
		spotLightMap_[i] = *light;
		i++;
	}



#ifdef _DEBUG
	for (int i = 0; const auto & [key, light] : spotLights_)
	{
		DebugRender.DrawSphere(light->pos, 5.0f, 0x00ff00, 255.0f, true);
	}
#endif
}

void LightController::SetBeforeDraw()
{
	DxCommand->SetGraphicsRootDescriptorTable(3, HeapAllocator.GetCBV_SRV_UAVGPUAddress(lightNumHeapNo_));

	DxCommand->SetGraphicsRootDescriptorTable(4, HeapAllocator.GetCBV_SRV_UAVGPUAddress(hemisphereLightHeapNo_));

	DxCommand->SetGraphicsRootDescriptorTable(5, HeapAllocator.GetCBV_SRV_UAVGPUAddress(directionalLightHeapNo_));

	DxCommand->SetGraphicsRootDescriptorTable(6, HeapAllocator.GetCBV_SRV_UAVGPUAddress(pointLightHeapNo_));

	DxCommand->SetGraphicsRootDescriptorTable(7, HeapAllocator.GetCBV_SRV_UAVGPUAddress(spotLightHeapNo_));
}

void LightController::AddHemisphereLight(HEMISPHERE_LIGHT light, const string& key, bool isCalcMat)
{
	if (HEMISPHERE_LIGHT_MAX_NUM <= hemisphereLights_.size() || hemisphereLights_.contains(key))return;

	if (isCalcMat) {
		hemisphereLightCameraProjMatCalcLights_.emplace_back(key);
	}

	hemisphereLights_[key] = move(make_unique<HEMISPHERE_LIGHT>(light));
}

void LightController::AddDirectionalLight(DIRECTIONAL_LIGHT light, const string& key, bool isCalcMat)
{
	if (DIRECTIONAL_LIGHT_MAX_NUM <= directionalLights_.size() || directionalLights_.contains(key))return;

	if (isCalcMat) {
		directionalLightCameraProjMatCalcLights_.emplace_back(key);
	}

	directionalLights_[key] = move(make_unique<DIRECTIONAL_LIGHT>(light));
}

void LightController::AddPointLight(POINT_LIGHT light, const string& key, bool isCalcMat)
{
	if (POINT_LIGHT_MAX_NUM <= pointLights_.size() || pointLights_.contains(key))return;

	if (isCalcMat) {
		pointLightCameraProjMatCalcLights_.emplace_back(key);
	}

	pointLights_[key] = move(make_unique<POINT_LIGHT>(light));
}

void LightController::AddSpotLight(SPOT_LIGHT light, const string& key, bool isCalcMat)
{
	if (SPOT_LIGHT_MAX_NUM <= spotLights_.size() || spotLights_.contains(key))return;

	if (isCalcMat) {
		spotLightCameraProjMatCalcLights_.emplace_back(key);
	}

	spotLights_[key] = move(make_unique<SPOT_LIGHT>(light));
}

void LightController::SetHemisphereLightInfo(HEMISPHERE_LIGHT light, const string& key)
{
	if (!hemisphereLights_.contains(key))return;

	*hemisphereLights_[key] = light;
}

void LightController::SetDirectionalLightInfo(DIRECTIONAL_LIGHT light, const string& key)
{
	if (!directionalLights_.contains(key))return;

	*directionalLights_[key] = light;
}

void LightController::SetPointLightInfo(POINT_LIGHT light, const string& key)
{
	if (!pointLights_.contains(key))return;

	*pointLights_[key] = light;
}

void LightController::SetSpotLightInfo(SPOT_LIGHT light, const string& key)
{
	if (!spotLights_.contains(key))return;

	*spotLights_[key] = light;
}

void LightController::DeleteHemisphereLightInfo(const string& key)
{
	if (hemisphereLights_.contains(key))
	{
		hemisphereLights_.erase(key);
	}

	erase_if(hemisphereLightCameraProjMatCalcLights_, [&key](const string& light)
		{
			return key == light;
		});
}

void LightController::DeleteDirectionalLightInfo(const string& key)
{
	if (directionalLights_.contains(key))
	{
		directionalLights_.erase(key);
	}

	erase_if(directionalLightCameraProjMatCalcLights_, [&key](const string& light)
		{
			return key == light;
		});
}

void LightController::DeletePointLightInfo(const string& key)
{
	if (pointLights_.contains(key))
	{
		pointLights_.erase(key);
	}

	erase_if(pointLightCameraProjMatCalcLights_, [&key](const string& light)
		{
			return key == light;
		});
}

void LightController::DeleteSpotLightInfo(const string& key)
{
	if (spotLights_.contains(key))
	{
		spotLights_.erase(key);
	}

	erase_if(spotLightCameraProjMatCalcLights_, [&key](const string& light)
		{
			return key == light;
		});
}

const XMMATRIX& LightController::GetLightCameraProjMat(int no) const
{
	no = min(no, LIGHT_CAMERA_MAX);
	no = max(no, 0);

	return lightNumMap_->lightCameraViewMatrix[no];
}

void LightController::CalculateHemisphereLightViewProjectionMatrix()
{
}

void LightController::CalculateDirectionalLightViewProjectionMatrix()
{
}

void LightController::CalculatePointLightViewProjectionMatrix()
{
}

void LightController::CalculateSpotLightViewProjectionMatrix()
{
	if (useLightCameraNum_ >= LIGHT_CAMERA_MAX)return;

	for (const auto& key : spotLightCameraProjMatCalcLights_)
	{
		const auto& light = *spotLights_.at(key);

		XMFLOAT3 eye(light.pos.x, light.pos.y, light.pos.z);
		Position3D targetPos = light.pos + light.direction * 100.0f;
		XMFLOAT3 target(targetPos.x, targetPos.y, targetPos.z);
		Vector3 upVec = Quaternion::LookRotation(light.direction).GetUp();
		XMFLOAT3 up(upVec.x, upVec.y, upVec.z);
		
		XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(light.angle * 3.0f,
			SCREEN_WIDTH / SCREEN_HEIGHT,//アス比
			1.0f,//近い方
			light.range * 10.0f//遠い方
		);
		//projMatrix = XMMatrixOrthographicLH(300, 300, 1.0f, light.range);

#ifdef _DEBUG
		DebugRender.DrawSphere(targetPos, 5.0f, 0xff0000, 255.0f, true);
	
#endif
		
		lightNumMap_->lightCameraViewMatrix[useLightCameraNum_] = viewMatrix;
		lightNumMap_->lightCameraProjMatrix[useLightCameraNum_] = projMatrix;
		lightNumMap_->lightCameraMatrix[useLightCameraNum_] = viewMatrix * projMatrix;
		useLightCameraNum_++;

		if (useLightCameraNum_ >= LIGHT_CAMERA_MAX)
		{
			break;
		}
	}

}
