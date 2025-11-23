#include "CommandAllocator.h"

void CommandAllocator::CommandAllocatorCreat(ID3D12Device* device)
{
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
}

void CommandAllocator::CommandAllocatorReset(ID3D12GraphicsCommandList* commandList, ID3D12Resource* renderTarget, ID3D12DescriptorHeap* rtvHeap, ID3D12Device* device, UINT64 currentBackBuffer, UINT rtvDescriptorSize, ID3D12PipelineState* piplineObj, ID3D12RootSignature* rootSignature, D3D12_VERTEX_BUFFER_VIEW* vertextBuffer, D3D12_INDEX_BUFFER_VIEW* indexBuffer)
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
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrier);

	// レンダーターゲットの取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_DESCRIPTOR_HEAP_TYPE rtvType = rtvHeap->GetDesc().Type;
	rtvHandle.ptr += currentBackBuffer * device->GetDescriptorHandleIncrementSize(rtvType);

	D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { rtvHandle };
	commandList->OMSetRenderTargets(1, handles, false, nullptr);

	// 画面クリア（青色）
	float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	commandList->SetPipelineState(piplineObj);
	commandList->SetGraphicsRootSignature(rootSignature);

	//設定
	D3D12_VIEWPORT viewPort = {};
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = 1280; //描画サイズ横
	viewPort.Height = 720; //描画サイズ縦
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	commandList->RSSetViewports(1, &viewPort);

	//シザー
	D3D12_RECT scissorRect = {};
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = 1280;
	scissorRect.bottom = 720;
	commandList->RSSetScissorRects(1, &scissorRect);

	commandList->IASetVertexBuffers(0, 1, vertextBuffer);
	commandList->IASetIndexBuffer(indexBuffer);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->DrawInstanced(3, 1, 0, 0);


	// コマンドリストをクローズ
	commandList->Close();
}