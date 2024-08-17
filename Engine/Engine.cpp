#include "pch.h"
#include "Engine.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Light.h"
#include "Resources.h"
#include "RenderTargetGroup.h"


void Engine::Init(const WindowInfo& info)
{
	_window = info;

	// �׷��� ȭ�� ũ�⸦ ����
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init(256);

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams), 256);

	CreateRenderTargetGroups();

	ResizeWindow(info.width, info.height);

	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();
	GET_SINGLE(Resources)->Init();
	GET_SINGLE(SceneManager)->InitLayers();

}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();

	Render();

	ShowFps();
}

void Engine::Render()
{
	RenderBegin();

	GET_SINGLE(SceneManager)->Render();

	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	RECT rect = { 0,0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

	//_depthStencilBuffer->Init(_window);
}

void Engine::ShowFps()
{
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_window.hwnd, text);
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	// ���������� reg�� �Ҵ��ϴ��� 
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}

shared_ptr<RenderTargetGroup> Engine::GetRTGroup(RENDER_TARGET_GROUP_TYPE type, uint32 layer)
{
	assert(layer < DEPTH_PEELING_LAYER_COUNT);
	
	switch (type)
	{
		case (RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN) : return GetSwapChainRTGroup();
		case (RENDER_TARGET_GROUP_TYPE::G_BUFFER)   : return _objectRTGroups[layer];
		case (RENDER_TARGET_GROUP_TYPE::LIGHTING)   : return _lightRTGroups[layer];
		case (RENDER_TARGET_GROUP_TYPE::LAYER_END)  : return _layerRTGroups[layer];
		default : return nullptr;
	}
}

shared_ptr<Texture> Engine::GetCurDepthReadTexture()
{
	if (curLayerIndex == 0)
	{
		return _zeroDepthTexture;
	}
	else
	{
		return _objectRTGroups[curLayerIndex - 1]->GetRTTexture(3);
	}
}

void Engine::CreateRenderTargetGroups()
{
	// DepthStencil
	shared_ptr<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, _window.width, _window.height,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// SwapChain Group
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);

		for (uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			ComPtr<ID3D12Resource> resource;
			_swapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resource));
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, resource);
		}

		_swapChainRTGroup = make_shared<RenderTargetGroup>();
		_swapChainRTGroup->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Depth Peeling ��, ù layer�� depth �񱳿����� �� ���� texture
	{
		_zeroDepthTexture = GET_SINGLE(Resources)->CreateTexture(L"InitialZeroDepthBuffer",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, Vec4(0.f, 0.f, 0.f, 0.f));
	}

	// Deferred Group
	{
		auto positionRT = GET_SINGLE(Resources)->CreateTexture(L"PositionTarget",
				DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		auto normalRT = GET_SINGLE(Resources)->CreateTexture(L"NormalTarget",
				DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		auto diffuseRT = GET_SINGLE(Resources)->CreateTexture(L"DiffuseTarget",
				DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		for (uint32 i = 0; i < DEPTH_PEELING_LAYER_COUNT; i++)
		{
			vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

			rtVec[0].target = positionRT;
			rtVec[1].target = normalRT;
			rtVec[2].target = diffuseRT;

			wstring textureName = L"AdditionalDepthBuffer_" + std::to_wstring(i);
			rtVec[3].target = GET_SINGLE(Resources)->CreateTexture(textureName,
				DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, Vec4(1.f, 0.f, 0.f, 0.f));
			rtVec[3].clearColor[0] = 1.0f;

			_objectRTGroups[i] = make_shared<RenderTargetGroup>();
			_objectRTGroups[i]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
		}
	}

	// Lighting Group
	{
		auto lightDiffuseRT = GET_SINGLE(Resources)->CreateTexture(L"DiffuseLightTarget",
				DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		auto lightSpecularRT = GET_SINGLE(Resources)->CreateTexture(L"SpecularLightTarget",
				DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		for (uint32 i = 0; i < DEPTH_PEELING_LAYER_COUNT; i++)
		{
			vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

			rtVec[0].target = lightDiffuseRT;
			rtVec[1].target = lightSpecularRT;

			_lightRTGroups[i] = make_shared<RenderTargetGroup>();
			_lightRTGroups[i]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
		}

	}

	// Layer Final Group
	{
		for (uint32 i = 0; i < DEPTH_PEELING_LAYER_COUNT; i++)
		{
			vector<RenderTarget> rtVec(RENDER_TARGET_LAYER_GROUP_MEMBER_COUNT);

			wstring textureName = L"LayerRenderResult_" + std::to_wstring(i);

			rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(textureName,
				DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

			_layerRTGroups[i] = make_shared<RenderTargetGroup>();
			_layerRTGroups[i]->Create(RENDER_TARGET_GROUP_TYPE::LAYER_END, rtVec, dsTexture);
		}
	}

	
}