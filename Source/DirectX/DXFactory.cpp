#include "DXFactory.h"

DXFactory::DXFactory()
{
}

DXFactory::~DXFactory()
{
}

void DXFactory::Terminate()
{
}

bool DXFactory::Create()
{
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
	auto result = CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(factory_.ReleaseAndGetAddressOf()));

	return !(FAILED(result));
}

const ComPtr<IDXGIFactory6>& DXFactory::GetFactory()
{
    if (factory_ == nullptr)
    {
        if (!Create())
        {
            assert(0 && "FactoryçÏê¨é∏îs");
        }
    }
    return factory_;
}
