#pragma once
#include <cassert>

#include <d3d12.h>

#include "O_DirectX12.h"

/// <summary>
/// Fence FenceEvent 
/// </summary>
class O_Fence
{
private:
	ID3D12Fence* fence;
	HANDLE fenceEvent;
	UINT64 fenceValue [2] = { 0, 0 };
	UINT64 nextValue = 0;

public:
	O_Fence() = default;
	~O_Fence();

	[[nodiscard]] bool Create(ID3D12Device* device);
	void Wait(UINT index);
	void Present(UINT index, O_DirectX12& directX);
};

