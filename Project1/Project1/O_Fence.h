#pragma once
#include <cassert>

#include "O_DirectX12.h"

/// <summary>
/// Fence FenceEvent 
/// </summary>
class O_Fence
{
private:
	ID3D12Fence* fence{};
	HANDLE fenceEvent{};
	UINT64 fenceValue [2] = { 0, 0 };
	UINT64 nextValue = 1;

public:
	O_Fence() = default;
	~O_Fence();

	[[nodiscard]] bool Create(O_DirectX12& direct);
	void Wait(UINT index);
	void Present(UINT index, O_DirectX12& direct);
};

