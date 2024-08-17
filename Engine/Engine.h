#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "RenderTargetGroup.h"

class Engine
{

public:

	void Init(const WindowInfo& info);
	void Update();
	
public:
	const WindowInfo& GetWindow() { return _window; }
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	shared_ptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
	shared_ptr<RenderTargetGroup> GetSwapChainRTGroup() { return _swapChainRTGroup; }
	shared_ptr<RenderTargetGroup> GetRTGroup(RENDER_TARGET_GROUP_TYPE type, uint32 layer);

public:
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

public:
	void SetLayerIndex(uint32 layer) { curLayerIndex = layer; }
	shared_ptr<Texture> GetCurDepthReadTexture();

private:
	void ShowFps();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	void CreateRenderTargetGroups();
 
private:
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<CommandQueue> _cmdQueue = make_shared<CommandQueue>();
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();
	shared_ptr<TableDescriptorHeap> _tableDescHeap = make_shared<TableDescriptorHeap>();

private:
	
	vector<shared_ptr<ConstantBuffer>> _constantBuffers;

	uint32 curLayerIndex = 0;
	shared_ptr<RenderTargetGroup> _swapChainRTGroup;
	shared_ptr<Texture> _zeroDepthTexture;
	array<shared_ptr<RenderTargetGroup>, DEPTH_PEELING_LAYER_COUNT> _objectRTGroups;
	array<shared_ptr<RenderTargetGroup>, DEPTH_PEELING_LAYER_COUNT> _lightRTGroups;
	array<shared_ptr<RenderTargetGroup>, DEPTH_PEELING_LAYER_COUNT> _layerRTGroups;
	//array<shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> _rtGroups;

	// Depth Peeling
	//array<shared_ptr<Texture>, DEPTH_PEELING_LAYER_COUNT> _layeredRenderResults;
	//array<shared_ptr<Texture>, DEPTH_PEELING_LAYER_COUNT> _peeledDepthBuffers;
	//array<shared_ptr<Texture>, ADDITIONAL_DEPTH_BUFFER_COUNT> _peeledDepthBuffers;

};

