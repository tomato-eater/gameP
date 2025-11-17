#include "DirectClear.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

ID3D12DescriptorHeap* DirectClear::DiscripterHeapDesc(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 2;  // スワップチェーンのバッファ数
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* rtvHeap;
	HRESULT hr = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	if (FAILED(hr))
	{
		hr = device->GetDeviceRemovedReason(); //DXGI_ERROR_DEVICE_HUNG
		return nullptr;
	}

	return rtvHeap;
}

UINT DirectClear::DiscripterHeapAccess(ID3D12Device* device, ID3D12DescriptorHeap* rtvHeap)
{
	// RTVハンドルの取得例
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 2番目のRTVハンドルの計算
	rtvHandle.ptr += rtvDescriptorSize;

	return rtvDescriptorSize;
}


ID3D12Resource* DirectClear::RenderTarget(IDXGISwapChain3* swapChain, ID3D12DescriptorHeap* rtvHeap, UINT rtvDescriptorSize, ID3D12Device* device)
{
	// レンダーターゲット作成例
	ID3D12Resource* renderTargets[2];  // スワップチェーンでバッファが二つあるのでダブルバッファで

	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));

		// RTVハンドルの計算
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += i * rtvDescriptorSize;

		// RTVの作成
		device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);
	}

	return renderTargets[0];
}


ID3D12CommandAllocator* DirectClear::CommandAllocatorCreat(ID3D12Device* device)
{
	// コマンドアロケータ作成例
	ID3D12CommandAllocator* commandAllocators[2];

	for (int i = 0; i < 2; i++)
	{
		device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i]));
	}

	return commandAllocators[0];
}


ID3D12GraphicsCommandList* DirectClear::CommandList(ID3D12Device* device, ID3D12CommandAllocator* commandAllocators)
{
	// コマンドリスト作成例
	ID3D12GraphicsCommandList* commandList;
	device->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT,commandAllocators, nullptr, IID_PPV_ARGS(&commandList));

	// 初期状態でクローズしておく
	commandList->Close();

	return commandList;
}
void DirectClear::CommandQueue(ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList)
{
	// 「画面をクリアする」コマンドリストを実行 → GPU が記録内容の実行を開始する
	commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&commandList);
}

void DirectClear::CommandAllocatorReset(IDXGISwapChain3* swapChain, ID3D12CommandAllocator* commandAllocators[2], ID3D12GraphicsCommandList* commandList, ID3D12Resource* renderTargets[2], ID3D12DescriptorHeap* rtvHeap, UINT rtvDescriptorSize)
{
	// 現在のバックバッファインデックスを取得
	UINT currentBackBuffer = swapChain->GetCurrentBackBufferIndex();

	// コマンドリストをリセット
	commandAllocators[currentBackBuffer]->Reset();
	commandList->Reset(commandAllocators[currentBackBuffer], nullptr);

	// レンダーターゲットを「絵を描く状態」に変更する（リソースバリア）
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = renderTargets[currentBackBuffer];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrier);

	// レンダーターゲットの取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += currentBackBuffer * rtvDescriptorSize;

	// 画面クリア（青色）
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	// レンダーターゲットを「絵を出す状態」に変更する（リソースバリア）
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	commandList->Close();
}

ID3D12Fence* DirectClear::FenceCreat(ID3D12Device* device)
{
	// フェンスの生成例
	ID3D12Fence* fence;
	UINT64 fenceValue = 0;

	// フェンスの生成
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	// フェンスイベントの生成
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return fence;
}

void DirectClear::FenceCheck(UINT64 fenceValue, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence)
{
	// GPU で行われているコマンドリストの処理が全て終わったら 1 になる様に設定する
	fenceValue = 1;
	commandQueue->Signal(fence, fenceValue);

	// まだ 1 になっていない（GPU処理が終わっていない）
	if (fence->GetCompletedValue() < fenceValue)
	{
		// 1 になった時に fenceEvent を行わせる
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		// fenceEvent が行われるまで CPU を止める
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}