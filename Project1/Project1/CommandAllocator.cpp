#include "CommandAllocator.h"

void CommandAllocator::CommandAllocatorCreat(ID3D12Device* device)
{
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
}

D3D12_RESOURCE_BARRIER CommandAllocator::CommandAllocatorReset(ID3D12GraphicsCommandList* commandList, 
								ID3D12Resource* renderTarget, ID3D12DescriptorHeap* rtvHeap, UINT64 currentBackBuffer, UINT rtvDescriptorSize)
{
	// コマンドリストをリセット
	commandAllocator->Reset();
	commandList->Reset(commandAllocator, nullptr);

	// レンダーターゲットを「絵を描く状態」に変更する（リソースバリア）
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = renderTarget;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrier);

	// レンダーターゲットの取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += currentBackBuffer * rtvDescriptorSize;

	if (!currentBackBuffer)
	{
			// 画面クリア（青色）
		float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	}
	else
	{
		// 画面クリア（赤色）
		float clearColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	}

	// レンダーターゲットを「絵を出す状態」に変更する（リソースバリア）
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	commandList->Close();

	return barrier;
}