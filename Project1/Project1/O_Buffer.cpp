#include "O_Buffer.h"

//頂点、指数の生成
bool O_Buffer::Create(O_DirectX12& directX)
{
	if (CreateVertex(directX.GetDevice())) return true;

	if (CreateIndex(directX.GetDevice())) return true;

	return false;
}

//頂点の位置と色のデータを入れる型
struct VertexSet
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
}; 

//ヒープの初期化
void SetHeap(D3D12_HEAP_PROPERTIES* heap)
{
	heap->Type = D3D12_HEAP_TYPE_UPLOAD;
	heap->CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap->MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heap->CreationNodeMask = 1;
	heap->VisibleNodeMask = 1;
}

//デスクの初期化
void SetDesc(D3D12_RESOURCE_DESC* desc, UINT64 size)
{
	desc->Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc->Alignment = 0;
	desc->Width = size;
	desc->Height = 1;
	desc->DepthOrArraySize = 1;
	desc->MipLevels = 1;
	desc->Format = DXGI_FORMAT_UNKNOWN;
	desc->SampleDesc.Count = 1;
	desc->SampleDesc.Quality = 0;
	desc->Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc->Flags = D3D12_RESOURCE_FLAG_NONE;
}

//頂点バッファーの生成
bool O_Buffer::CreateVertex(ID3D12Device* device)
{
	VertexSet vertices[] =
	{
		{{ 0.5f, 0.5f, 0.0f},{ 1.0f, 0.0f, 0.0f, 1.0f}},
		{{ 0.5f,-0.5f, 0.0f},{ 0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f},{ 0.0f, 0.0f, 1.0f, 1.0f}},
		{{-0.5f,-0.5f, 0.0f},{ 1.0f, 1.0f, 1.0f, 1.0f}}
	};

	D3D12_HEAP_PROPERTIES heapPro = {};
	SetHeap(&heapPro);

	D3D12_RESOURCE_DESC reDesc = {};
	SetDesc(&reDesc, sizeof(vertices));

	ID3D12Resource* vertexBuffer;
	
	HRESULT hr = device->CreateCommittedResource
	(
		&heapPro, D3D12_HEAP_FLAG_NONE,
		&reDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertexBuffer)
	);
	if (FAILED(hr))
	{
		assert(false && "頂点バッファの生成に失敗");
		return true;
	}

	VertexSet* data{};
	hr = vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(hr))
	{
		assert(false && "頂点バッファのマップに失敗");
		return  true;
	}

	memcpy(data, vertices, sizeof(vertices));
	vertexBuffer->Unmap(0, nullptr);

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(VertexSet);
	vertexBufferView.SizeInBytes = sizeof(vertices);

	return false;
}

//指数バッファーの生成
bool O_Buffer::CreateIndex(ID3D12Device* device)
{
	UINT16 indices[] = { 0, 1, 2, 1, 2, 3 };

	D3D12_HEAP_PROPERTIES heapPro = {};
	SetHeap(&heapPro);

	D3D12_RESOURCE_DESC reDesc = {};
	SetDesc(&reDesc, sizeof(indices));

	ID3D12Resource* indexBuffer;
	HRESULT hr = device->CreateCommittedResource
	(
		&heapPro, D3D12_HEAP_FLAG_NONE,
		&reDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&indexBuffer)
	);
	if (FAILED(hr))
	{
		assert(false && "指数バッファの生成に失敗");
		return true;
	}

	uint16_t* data{};
	hr = indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(hr))
	{
		assert(false && "指数バッファのマップに失敗");
		return true;
	}

	memcpy(data, indices, sizeof(indices));
	indexBuffer->Unmap(0, nullptr);

	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView.SizeInBytes = sizeof(indices);

	return false;
}