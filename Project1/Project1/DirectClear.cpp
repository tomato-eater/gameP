#include "DirectClear.h"

#include <d3d12.h>

D3D12_DESCRIPTOR_HEAP_DESC* DirectClear::DiscripterHeapDesc()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;  // スワップチェーンのバッファ数
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* rtvHeap;
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}