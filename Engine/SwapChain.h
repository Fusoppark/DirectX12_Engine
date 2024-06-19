#pragma once

// - Double Buffering

// - [0] [1]
// 현재 화면 [1]  <-> GPU 작업중 [1] BackBuffer

class SwapChain
{
public :
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	
	void Present();
	void SwapIndex();

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }
	uint8 GetBackBufferIndex() { return _backBufferIndex; }
	

private :
	void InitSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);

private :
	ComPtr<IDXGISwapChain>	_swapChain;

	uint32					_backBufferIndex = 0;
};

