#include "DirectClear.h"

#include <d3d12.h>
#include <dxgi1_4.h>
//DirectClear::
ID3D12DescriptorHeap* DiscripterHeapDesc(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;  // スワップチェーンのバッファ数
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* rtvHeap;
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	return rtvHeap;
}

UINT* DiscripterHeapAccess(ID3D12Device* device, ID3D12DescriptorHeap* rtvHeap)
{
	// RTVハンドルの取得例
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 2番目のRTVハンドルの計算
	rtvHandle.ptr += rtvDescriptorSize;

	return &rtvDescriptorSize;
}


void RenderTarget(IDXGISwapChain3* swapChain, ID3D12DescriptorHeap* rtvHeap, UINT* rtvDescriptorSize, ID3D12Device* device)
{
	// レンダーターゲット作成例
	ID3D12Resource* renderTargets[2];  // スワップチェーンでバッファが二つあるのでダブルバッファで

	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));

		// RTVハンドルの計算
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += i * *rtvDescriptorSize;

		// RTVの作成
		device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);
	}
}


ID3D12CommandAllocator* CommandAllocatorCreat(ID3D12Device* device)
{
	// コマンドアロケータ作成例
	ID3D12CommandAllocator* commandAllocators[2];

	for (int i = 0; i < 2; i++)
	{
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&commandAllocators[i])
		);
	}

	return commandAllocators[0];
}

ID3D12GraphicsCommandList* CommandList(ID3D12Device* device, ID3D12CommandAllocator* commandAllocators)
{
	// コマンドリスト作成例
	ID3D12GraphicsCommandList* commandList;
	device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocators[currentBackBuffer],
		nullptr,
		IID_PPV_ARGS(&commandList)
	);

	// 初期状態でクローズしておく
	commandList->Close();

	return commandList;
}

void CommandQueue(ID3D12GraphicsCommandList* commandList)
{
	// 「画面をクリアする」コマンドリストを実行 → GPU が記録内容の実行を開始する
	commandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&commandList);
}