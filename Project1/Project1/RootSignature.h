#pragma once

#include <d3d12.h>
class RootSignature
{
public:
	RootSignature() {};
	~RootSignature() {};

	ID3D12RootSignature* RootSignatureCreate(ID3D12Device* device);
};

