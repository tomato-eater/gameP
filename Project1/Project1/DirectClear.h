#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
class DirectClear
{
private:
	HANDLE fenceEvent;

public:
	DirectClear() {}
	~DirectClear() {}

	ID3D12DescriptorHeap* DiscripterHeapDesc(ID3D12Device* device);
	UINT DiscripterHeapAccess(ID3D12Device* device, ID3D12DescriptorHeap* rtvHeap);
	ID3D12Resource* RenderTarget(IDXGISwapChain3* swapChain, ID3D12DescriptorHeap* rtvHeap, UINT rtvDescriptorSize, ID3D12Device* device);
	ID3D12CommandAllocator* CommandAllocatorCreat(ID3D12Device* device);
	ID3D12GraphicsCommandList* CommandList(ID3D12Device* device, ID3D12CommandAllocator* commandAllocators);
	void CommandQueue(ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList);
	void CommandAllocatorReset(IDXGISwapChain3* swapChain, ID3D12CommandAllocator* commandAllocators[2], ID3D12GraphicsCommandList* commandList, ID3D12Resource* renderTargets[2], ID3D12DescriptorHeap* rtvHeap, UINT rtvDescriptorSize);
	ID3D12Fence* FenceCreat(ID3D12Device* device);
	void FenceCheck(UINT64 fenceValue, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence);
};

