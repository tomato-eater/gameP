#pragma once
#include <cassert>

#include "O_DirectX12.h"
class O_Signature
{
private:
	ID3D12RootSignature* rootSignature;

public:
	O_Signature() = default;
	~O_Signature();

	[[nodiscard]] bool Create(O_DirectX12& directX);

	bool CreateSignature(ID3D12Device* device);

	ID3D12RootSignature* GetRoot() { return rootSignature; }
};

