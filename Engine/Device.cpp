#include "pch.h"
#include "Device.h"

void Device::Init()
{
	// Result Handler
	HRESULT hr = {};

	// D3D12 디버그층 활성화
#ifdef _DEBUG
	hr = ::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));

	if (SUCCEEDED(hr))
	{
		_debugController->EnableDebugLayer();
	}
	else 
	{
		Log::Warning("Device.Init() : D3D12GetDebugInterface() failed : " + hr);
	}

#endif

	// DXGI(DirectX Graphics Infrastructure)
	hr = ::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));
#ifdef _DEBUG
	if (!SUCCEEDED(hr))
	{
		Log::Error("Device.Init() : CreateDXGIFactory() failed : " + hr);
	}
#endif

	// CreateDevice
	
	hr = ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
#ifdef _DEBUG
	if (!SUCCEEDED(hr))
	{
		Log::Error("Device.Init() : D3D12CreateDevice() failed : " + hr);
	}
#endif
}
