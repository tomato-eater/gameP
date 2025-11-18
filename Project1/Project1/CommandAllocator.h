#pragma once

#include <d3d12.h>

class CommandAllocator
{
private:
	ID3D12CommandAllocator* commandAllocator;

public:
	CommandAllocator() {}
	~CommandAllocator() {}

	void CommandAllocatorCreat(ID3D12Device* device);
	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator; }
	D3D12_RESOURCE_BARRIER CommandAllocatorReset(ID3D12GraphicsCommandList* commandList,
							ID3D12Resource* renderTarget, ID3D12DescriptorHeap* rtvHeap, UINT64 currentBackBuffer, UINT rtvDescriptorSize);
};

