#pragma once
#include <cassert>

#include "O_DirectX12.h"
#include "O_Signature.h"
#include "O_Shader.h"

class O_PipLine
{
private:
	ID3D12PipelineState* pipLine;

public:
	O_PipLine() = default;
	~O_PipLine();

	[[nodiscard]] bool Create(O_DirectX12& directX, O_Signature& signature, O_Shader& shader);

	bool CreatePipLine(ID3D12Device* device, ID3D12RootSignature* signature, ID3D10Blob* vs, ID3D10Blob* ps);

	ID3D12PipelineState* GetPipLine() { return pipLine; }
};

