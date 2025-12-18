#include "O_DeltaPolygon.h"

struct Vertex
{
	XMFLOAT3 positiaon;
	XMFLOAT4 color;
};

O_DeltaPolygon::~O_DeltaPolygon()
{
	if (verBuffer)
	{
		verBuffer->Release();
		verBuffer = nullptr;
	}

	if (indBuffer)
	{
		indBuffer->Release();
		indBuffer = nullptr;
	}
}

bool O_DeltaPolygon::CreateBuffers(O_DirectX12& direct)
{
	if (CreateVertex(direct.GetDevice())) return true;

	if (CreateIndex(direct.GetDevice())) return true;

	return false;
}

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


bool O_DeltaPolygon::CreateVertex(ID3D12Device* device)
{
	Vertex vertices[] =
	{
		{{ 0.0f,  0.5f,  0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f,  0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f}}
	};

	D3D12_HEAP_PROPERTIES heapPro = {};
	SetHeap(&heapPro);

	D3D12_RESOURCE_DESC reDesc = {};
	SetDesc(&reDesc, sizeof(vertices));

	HRESULT hr = device->CreateCommittedResource
	(
		&heapPro, D3D12_HEAP_FLAG_NONE,
		&reDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&verBuffer)
	);
	if (FAILED(hr))
	{
		assert(false && "頂点バッファの生成に失敗");
		return true;
	}

	Vertex* data{};

	hr = verBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(hr))
	{
		assert(false && "頂点バッファのマップに失敗");
		return  true;
	}

	memcpy_s(data, sizeof(vertices), vertices, sizeof(vertices));
	verBuffer->Unmap(0, nullptr);

	verView.BufferLocation = verBuffer->GetGPUVirtualAddress();
	verView.SizeInBytes = sizeof(vertices);
	verView.StrideInBytes = sizeof(Vertex);

	return false;
}

bool O_DeltaPolygon::CreateIndex(ID3D12Device* device)
{
	uint16_t deltaInd[] = { 0,1,2 };

	D3D12_HEAP_PROPERTIES heapPro = {};
	SetHeap(&heapPro);

	D3D12_RESOURCE_DESC reDesc = {};
	SetDesc(&reDesc, sizeof(deltaInd));

	HRESULT hr = device->CreateCommittedResource
	(
		&heapPro, D3D12_HEAP_FLAG_NONE,
		&reDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&indBuffer)
	);
	if (FAILED(hr))
	{
		assert(false && "指数バッファの生成に失敗");
		return true;
	}

	uint16_t* data{};
	hr = indBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(hr))
	{
		assert(false && "指数バッファのマップに失敗");
		return true;
	}

	memcpy_s(data, sizeof(deltaInd), deltaInd, sizeof(deltaInd));
	indBuffer->Unmap(0, nullptr);

	indView.BufferLocation = indBuffer->GetGPUVirtualAddress();
	indView.SizeInBytes = sizeof(deltaInd);
	indView.Format = DXGI_FORMAT_R16_UINT;

	return false;
}

void O_DeltaPolygon::Draw(O_List& list)
{
	list.GetList()->IASetVertexBuffers(0, 1, &verView);
	list.GetList()->IASetIndexBuffer(&indView);
	list.GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	list.GetList()->DrawIndexedInstanced(3, 1, 0, 0, 0);
}