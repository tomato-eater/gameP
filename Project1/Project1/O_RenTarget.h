#pragma once

#include "O_DirectX12.h"
#include "O_Heap.h"

#include <vector>

class O_RenTarget
{
private:
	std::vector<ID3D12Resource*> renderTarget;

public:
	O_RenTarget() = default;
	~O_RenTarget();

	[[nodiscard]] bool CreateTarget(O_Heap& heap, O_DirectX12& direct);

	ID3D12Resource* GetTarget(UINT index) { return renderTarget[index]; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(UINT idx, O_Heap& heap, O_DirectX12& direct);
};

