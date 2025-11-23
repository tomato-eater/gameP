#pragma once

#include <d3d12.h>

class CommandAllocator
{
private:
	ID3D12CommandAllocator* commandAllocator;

public:
	CommandAllocator() = default;
	~CommandAllocator() {}

	void CommandAllocatorCreat(ID3D12Device* device);
	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator; }
	void CommandAllocatorReset(ID3D12GraphicsCommandList* commandList,ID3D12Resource* renderTarget, ID3D12DescriptorHeap* rtvHeap, ID3D12Device* device, UINT64 currentBackBuffer, UINT rtvDescriptorSize, ID3D12PipelineState* piplineObj, ID3D12RootSignature* rootSignature, D3D12_VERTEX_BUFFER_VIEW* vertextBuffer, D3D12_INDEX_BUFFER_VIEW* indexBuffer);
};

