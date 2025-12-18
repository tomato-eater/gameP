#pragma once

#include "O_DirectX12.h"

class O_Heap
{
private:
	ID3D12DescriptorHeap* heap;
	D3D12_DESCRIPTOR_HEAP_TYPE type;

public:
	O_Heap() = default;
	~O_Heap();

	[[nodiscard]] bool CreateHeap(O_DirectX12& direct, D3D12_DESCRIPTOR_HEAP_TYPE setType, UINT numDesc, bool shader);

	ID3D12DescriptorHeap* GetHeap() { return heap; }
	D3D12_DESCRIPTOR_HEAP_TYPE GetType() { return type; }
};

