#pragma once

#include "O_DirectX12.h"
#include "O_Allocator.h"
#include "O_RenTarget.h"

class O_List
{
private:
	ID3D12GraphicsCommandList* commandList;

public:
	O_List() = default;
	~O_List();

	[[nodiscard]] bool CreateList(O_DirectX12& direct, O_Allocator& allo);

	void ResetList(ID3D12CommandAllocator* allo) { commandList->Reset(allo, nullptr); }
	void SetBarriert(O_RenTarget& target, UINT idx, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to);

	ID3D12GraphicsCommandList* GetList() { return commandList; }
};

