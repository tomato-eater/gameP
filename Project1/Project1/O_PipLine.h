#pragma once

#include "O_DirectX12.h"
#include "O_Signature.h"
#include "O_Shader.h"

class O_PipLine
{
private:
	ID3D12PipelineState* pipLine{};

public:
	O_PipLine() = default;
	~O_PipLine();

	[[nodiscard]] bool Create(O_Signature& root, O_Shader& shader, O_DirectX12& direct);

	ID3D12PipelineState* GetPipLine() { return pipLine; }
};