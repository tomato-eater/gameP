#include "RenderTarget.h"

void RenderTarget::RenderTargetCreate(IDXGISwapChain3* swapChain, UINT i, ID3D12DescriptorHeap* rtvHeap, UINT rtvDescriptorSize, ID3D12Device* device )
{
	// スワップチェーンからバッファを取得
	swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget));

	// RTVハンドルの計算
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += i * rtvDescriptorSize;

	// RTVの作成
	device->CreateRenderTargetView(renderTarget, nullptr, rtvHandle);
}