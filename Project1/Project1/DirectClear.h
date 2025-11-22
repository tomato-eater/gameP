#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

class DirectClear
{
private:
	HANDLE fenceEvent;
	UINT64 fenceValue[2] = { 0, 0 };
	UINT64 nextValue = 0;

public:
	DirectClear() = default;
	~DirectClear() {}

	ID3D12DescriptorHeap* DiscripterHeapDesc(ID3D12Device* device);
	UINT DiscripterHeapAccess(ID3D12Device* device, ID3D12DescriptorHeap* rtvHeap);
	ID3D12GraphicsCommandList* CommandList(ID3D12Device* device, ID3D12CommandAllocator* commandAllocators);
	void CommandQueue(ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList);
	ID3D12Fence* FenceCreat(ID3D12Device* device);
	void FenceWait(UINT64 fenceValue, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence, IDXGISwapChain3* swapChain);
	void FencePresent(UINT64 currentBackBuffer, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence, IDXGISwapChain3* swapChain);
};

