#include "DirectClear.h"

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

ID3D12GraphicsCommandList* DirectClear::CommandList(ID3D12Device* device, ID3D12CommandAllocator* commandAllocators)
{
	// コマンドリスト作成例
	ID3D12GraphicsCommandList* commandList;
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators, nullptr, IID_PPV_ARGS(&commandList));
	// 初期状態でクローズしておく
	commandList->Close();

	return commandList;
}

void DirectClear::CommandQueue(ID3D12CommandQueue* commandQueue, ID3D12GraphicsCommandList* commandList)
{
	// 「画面をクリアする」コマンドリストを実行 → GPU が記録内容の実行を開始する
	commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&commandList);
}

ID3D12Fence* DirectClear::FenceCreat(ID3D12Device* device)
{
	// フェンスの生成例
	ID3D12Fence* fence;

	// フェンスの生成
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	// フェンスイベントの生成
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return fence;
}

void DirectClear::FenceWait(UINT64 currentBackBuffer, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence, IDXGISwapChain3* swapChain)
{
	// まだ 1 になっていない（GPU処理が終わっていない）
	if (fence->GetCompletedValue() < fenceValue[currentBackBuffer])
	{
		// 1 になった時に fenceEvent を行わせる
		fence->SetEventOnCompletion(fenceValue[currentBackBuffer], fenceEvent);
		// fenceEvent が行われるまで CPU を止める
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

void DirectClear::FencePresent(UINT64 currentBackBuffer, ID3D12CommandQueue* commandQueue, ID3D12Fence* fence, IDXGISwapChain3* swapChain)
{
	fenceValue[currentBackBuffer] = ++nextValue;
	commandQueue->Signal(fence, fenceValue[currentBackBuffer]);

	swapChain->Present(1, 0);
}