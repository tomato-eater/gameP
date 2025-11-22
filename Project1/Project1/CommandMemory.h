#pragma once

#include <d3d12.h>
#include "CommandAllocator.h"
class CommandMemory
{
public:
	CommandMemory() = default;
	~CommandMemory() {};

	void Memory(ID3D12GraphicsCommandList* commandList, CommandAllocator commandAllocator, ID3D12PipelineState* piplineObj);
};

