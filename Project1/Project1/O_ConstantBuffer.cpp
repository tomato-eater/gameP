#include "O_ConstantBuffer.h"

O_ConstantBuffer::~O_ConstantBuffer()
{
	if (consBuffer)
	{
		consBuffer->Release();
		consBuffer = nullptr;
	}
}

bool O_ConstantBuffer::CreateBuffer(O_DirectX12& direct, O_Heap& heap, UINT bufferSize, UINT index)
{
	D3D12_HEAP_PROPERTIES heapPro{};
	heapPro.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC reDesc{};
	reDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	reDesc.Width = (sizeof(bufferSize) + 255) & ~255;
	reDesc.Height = 1;
	reDesc.DepthOrArraySize = 1;
	reDesc.MipLevels = 1;
	reDesc.Format = DXGI_FORMAT_UNKNOWN;
	reDesc.SampleDesc.Count = 1;
	reDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = direct.GetDevice()->CreateCommittedResource
	(
		&heapPro, D3D12_HEAP_FLAG_NONE,
		&reDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&consBuffer)
	);
	if (FAILED(hr))
	{
		assert(false && "コンスタントバッファの生成に失敗");
		return true;
	}

	if (heap.GetType() != D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	{
		assert(false && "ヒープタイプが違う");
		return true;
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = consBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (sizeof(bufferSize) + 255) & ~255;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap.GetHeap()->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += index * direct.GetDevice()->GetDescriptorHandleIncrementSize(heap.GetType());

	direct.GetDevice()->CreateConstantBufferView(&cbvDesc, cpuHandle);
	gpuHundle = heap.GetHeap()->GetGPUDescriptorHandleForHeapStart();
	gpuHundle.ptr += index * direct.GetDevice()->GetDescriptorHandleIncrementSize(heap.GetType());

	return false;
}