#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

class RenderTarget
{
private: 
	ID3D12Resource* renderTarget;

public:
	RenderTarget() = default;
	~RenderTarget() {}

	void RenderTargetCreate(IDXGISwapChain3* swapChain, UINT i, ID3D12DescriptorHeap* rtvHeap, UINT rtvDescriptorSize, ID3D12Device* device);
	ID3D12Resource* GetRenderTarget() { return renderTarget; }
};

