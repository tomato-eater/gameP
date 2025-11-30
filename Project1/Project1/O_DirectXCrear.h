#pragma once
#include <cassert>

#include "O_DirectX12.h"

/// <summary>
/// Heap RenderTarget CommandAllocator CommandList
/// </summary>
class O_DirectXCrear
{
private:
	ID3D12DescriptorHeap* heap;
	ID3D12Resource* renderTarget[2];
	ID3D12CommandAllocator* commandAllocator[2];
	ID3D12GraphicsCommandList* commandList;

public:
	O_DirectXCrear() = default;
	~O_DirectXCrear();

	[[nodiscard]] bool Create(O_DirectX12& directX);

	bool CreateHeap(ID3D12Device* device);
	bool CreateTarget(IDXGISwapChain3* swapChain, ID3D12Device* device);
	bool CreateAllocator(ID3D12Device* device);
	bool CreateList(ID3D12Device* device);

	ID3D12DescriptorHeap* GetHeap() { return heap; }
	ID3D12Resource* GetTarget(int i) { return renderTarget[i]; }
	ID3D12CommandAllocator* GetAllocator(int i) { return commandAllocator[i]; }
	ID3D12GraphicsCommandList* GetList() { return commandList; }
};

