#include "DeltaBuffer.h"

#include<DirectXMath.h>
struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

D3D12_VERTEX_BUFFER_VIEW DeltaBuffer::VertexBufferCreate(ID3D12Device* device)
{
	Vertex triangleVertices[] =
	{
		{{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	};

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = sizeof(triangleVertices);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* vertexBuffer;
	device->CreateCommittedResource
	(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)
	);

	// 頂点バッファにデータを転送する
	// CPU からアクセス可能なアドレスを取得
	Vertex* data{};

	// バッファをマップ（CPUから更新開始）
	// vertexBuffer を直接利用するのではなく、data を介して更新するイメージ
	vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

	// 頂点データをコピー
	memcpy(data, triangleVertices, sizeof(triangleVertices));

	// コピーが終わったのでマップ解除（CPUから更新終了）
	// ここまで来たら GPU が利用するメモリ領域（VRAM）にコピー済みなので、triangleVertices は不要になる
	vertexBuffer->Unmap(0, nullptr);

	// 描画の際に利用するビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = sizeof(triangleVertices);

	return vertexBufferView;
}

D3D12_INDEX_BUFFER_VIEW DeltaBuffer::IndexBufferCreate(ID3D12Device* device)
{
	UINT16 triangleIndices[] = { 0, 1, 2 };

	D3D12_HEAP_PROPERTIES heapProperty = {};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = sizeof(triangleIndices);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* indexBuffer;
	device->CreateCommittedResource
	(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer)
	);

	uint16_t* data{};
	indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));
	memcpy_s(data, sizeof(triangleIndices), triangleIndices, sizeof(triangleIndices));
	indexBuffer->Unmap(0, nullptr);

	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView.SizeInBytes = sizeof(triangleIndices);

	return indexBufferView;
}