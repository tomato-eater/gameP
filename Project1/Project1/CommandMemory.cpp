#include "CommandMemory.h"

void CommandMemory::Memory(ID3D12GraphicsCommandList* commandList, CommandAllocator commandAllocator, ID3D12PipelineState* piplineObj)
{
	commandList->Reset(commandAllocator.GetCommandAllocator(), piplineObj);
	commandList->OMSetRenderTargets(1, commandAllocator.GetHandle(), FALSE, nullptr);

	const float clearColor[] = { 0.0f,0.2f,0.4f,1.0f };
	commandList->ClearRenderTargetView(*commandAllocator.GetHandle(), clearColor, 0, nullptr);


}