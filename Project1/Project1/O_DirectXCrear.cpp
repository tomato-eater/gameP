#include "O_DirectXCrear.h"

O_DirectXCrear::~O_DirectXCrear()
{
	if (heap)
	{
		heap->Release();
		heap = nullptr;
	}
}

bool O_DirectXCrear::Create(O_DirectX12& directX)
{
	if (CreateHeap(directX.GetDevice()))
	{
		assert(false && "ディスクリプタヒープの生成に失敗");
		return true;
	}

	if (CreateTarget(directX.GetSwapChain(), directX.GetDevice()))
	{
		assert(false && "レンダ―ターゲットの生成に失敗");
		return true;
	}

	if (CreateAllocator(directX.GetDevice()))
	{
		assert(false && "コマンドアロケータの生成に失敗");
		return true;
	}

	if (CreateList(directX.GetDevice()))
	{
		assert(false && "コマンドリストの生成に失敗");
		return true;
	}

	return false;
}

//ディスクリプタヒープの生成
bool O_DirectXCrear::CreateHeap(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = 2;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&heap));
	if (FAILED(hr)) return true;

	return false;
}

//レンダ―ターゲットの生成
bool O_DirectXCrear::CreateTarget(IDXGISwapChain3* swapChain, ID3D12Device* device)
{
	for (int i = 0; i < 2; i++)
	{
		HRESULT hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget[i]));
		if (FAILED(hr))return true;

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = heap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		device->CreateRenderTargetView(renderTarget[i], nullptr, rtvHandle);
	}

	return false;
}

//コマンドアロケータの生成
bool O_DirectXCrear::CreateAllocator(ID3D12Device* device)
{
	for (int i = 0; i < 2; i++)
	{
		HRESULT hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator[i]));
		if (FAILED(hr)) return true;
	}

	return false;
}

//コマンドリストの生成
bool O_DirectXCrear::CreateList(ID3D12Device* device)
{
	HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[0], nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr)) return true;

	commandList->Close();

	return false;
}