#include "O_Heap.h"

O_Heap::~O_Heap()
{
	if (heap)
	{
		heap->Release();
		heap = nullptr;
	}
}

bool O_Heap::CreateHeap(O_DirectX12& direct, D3D12_DESCRIPTOR_HEAP_TYPE setType, UINT numDesc, bool shader)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = setType;
	heapDesc.NumDescriptors = numDesc;
	heapDesc.Flags = shader ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	type = setType;

	HRESULT hr = direct.GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		assert(false && "ƒq[ƒv‚Ì¶¬‚É¸”s");
		return true;
	}

	return false;
}