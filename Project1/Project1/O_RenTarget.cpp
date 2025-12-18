#include "O_RenTarget.h"

O_RenTarget::~O_RenTarget()
{
	for (auto& tar : renderTarget)
		if (tar)
		{
			tar->Release();
			tar = nullptr;
		}
}

bool O_RenTarget::CreateTarget(O_Heap& heap, O_DirectX12& direct)
{
	const auto& desc = direct.GetSwapDesc();
	renderTarget.resize(desc.BufferCount);

	auto handle = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();

	assert(heap.GetType() == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません");

	for (int i = 0; i < desc.BufferCount; i++)
	{
		HRESULT hr = direct.GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&renderTarget[i]));
		if (FAILED(hr))
		{
			assert(false && "レンダ―ターゲットの生成に失敗");
			return true;
		}

		direct.GetDevice()->CreateRenderTargetView(renderTarget[i], nullptr, handle);
		handle.ptr += direct.GetDevice()->GetDescriptorHandleIncrementSize(heap.GetType());
	}

	return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE O_RenTarget::GetHandle(UINT idx, O_Heap& heap, O_DirectX12& direct)
{
	auto handle = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
	assert(heap.GetType() == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません");
	handle.ptr += idx * direct.GetDevice()->GetDescriptorHandleIncrementSize(heap.GetType());
	return handle;
}