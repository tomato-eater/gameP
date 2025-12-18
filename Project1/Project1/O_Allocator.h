#pragma once

#include "O_DirectX12.h"

class O_Allocator
{
private:
	ID3D12CommandAllocator* commandAllocator;

public:
	O_Allocator() = default;
	~O_Allocator();

	[[nodiscard]] bool CreateAllocator(O_DirectX12& direct);

	void ResetAllocator() { commandAllocator->Reset(); }

	ID3D12CommandAllocator* GetAllocator() { return commandAllocator; }
};

