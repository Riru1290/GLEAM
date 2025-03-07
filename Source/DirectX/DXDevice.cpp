#include "DXDevice.h"

DXDevice::DXDevice()
{
}

DXDevice::~DXDevice()
{
}

void DXDevice::Terminate()
{
}

bool DXDevice::Create()
{
	EnableDebugLayer();

	ComPtr<IDXGIAdapter> SelectAdapter = nullptr;
	vector<ComPtr<IDXGIAdapter>> Adapters;
	vector<DXGI_ADAPTER_DESC> descs;

	// 使用中PCにあるGPUドライバーを検索して、あれば格納する
	for (UINT index = 0; 1; ++index)
	{
		Adapters.push_back(nullptr);
		HRESULT ret = Factory->EnumAdapters(index, &Adapters[index]);

		if (ret == DXGI_ERROR_NOT_FOUND) { break; }

		descs.push_back({});
		Adapters[index]->GetDesc(&descs[index]);
	}


	GPUTier gpuTier = GPUTier::Kind;

	// 優先度の高いGPUドライバーを使用する
	for (int i = 0; i < descs.size(); ++i)
	{
		if (wstring(descs[i].Description).find(L"NVIDIA") != wstring::npos)
		{
			SelectAdapter = Adapters[i];
			break;
		}
		else if (wstring(descs[i].Description).find(L"Amd") != wstring::npos)
		{
			if (gpuTier > GPUTier::Amd)
			{
				SelectAdapter = Adapters[i];
				gpuTier = GPUTier::Amd;
			}
		}
		else if (wstring(descs[i].Description).find(L"Intel") != wstring::npos)
		{
			if (gpuTier > GPUTier::Intel)
			{
				SelectAdapter = Adapters[i];
				gpuTier = GPUTier::Intel;
			}
		}
		else if (wstring(descs[i].Description).find(L"Arm") != wstring::npos)
		{
			if (gpuTier > GPUTier::Arm)
			{
				SelectAdapter = Adapters[i];
				gpuTier = GPUTier::Arm;
			}
		}
		else if (wstring(descs[i].Description).find(L"Qualcomm") != wstring::npos)
		{
			if (gpuTier > GPUTier::Qualcomm)
			{
				SelectAdapter = Adapters[i];
				gpuTier = GPUTier::Qualcomm;
			}
		}
	}

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel = {};

	// Direct3Dデバイスの初期化
	HRESULT result = {};
	for (auto lv : levels)
	{
		result = D3D12CreateDevice(SelectAdapter.Get(), lv, IID_PPV_ARGS(&device_));
		if (result == S_OK)
		{
			featureLevel = lv;
			break;
		}
	}

	return !(FAILED(result));
}

const ComPtr<ID3D12Device>& DXDevice::GetDevice()
{
    if (device_ == nullptr)
    {
		if (!Create())
		{
			assert(0 && "Device作成失敗");
		}
    }
    return device_;
}
