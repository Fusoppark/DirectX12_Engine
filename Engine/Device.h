#pragma once
class Device
{
public:

	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	
	// COM = Component Object Model
	ComPtr<IDXGIFactory>		_dxgi;		
	ComPtr<ID3D12Device>		_device;	

	// Debug
	ComPtr<ID3D12Debug>			_debugController;
};

