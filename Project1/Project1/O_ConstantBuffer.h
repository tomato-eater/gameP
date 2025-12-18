#pragma once

#include "O_DirectX12.h"
#include "O_Heap.h"

class O_ConstantBuffer
{
private:
	ID3D12Resource* consBuffer{};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHundle{};

public:
	O_ConstantBuffer() = default;
	~O_ConstantBuffer();

	[[nodiscard]] bool CreateBuffer(O_DirectX12& direct, O_Heap& heap, UINT bufferSize, UINT index);

	ID3D12Resource* GetBuffer() { return consBuffer; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return gpuHundle; }
};

