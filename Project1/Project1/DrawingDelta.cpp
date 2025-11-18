#include "DrawingDelta.h"

#include<WindowsNumerics.h>
struct Vertex
{
    DirectX::XMFLOAT3 position;  // 頂点座標（x, y, z）
    DirectX::XMFLOAT4 color;     // 頂点色（r, g, b, a）
};

void DrawingDelta::TriangleVertices(ID3D12Device* device)
{
    Vertex triangleVertices[] =
    {
        // 頂点座標                  // 頂点色
        { DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }, // 頂点1: 赤
        { DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }, // 頂点2: 緑
        { DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }  // 頂点3: 青
	};

    // CPUからデータを送り込めるメモリを使いたい
    D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;

    // 頂点データ分のサイズの1次元バッファがほしい
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = sizeof(triangleVertices); // 頂点データ全体のサイズ
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// 上記の条件でリソースを作成
	device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)
	);

	// 頂点バッファにデータ転送
	Vertex* data{};

	// バッファをマップ（CPUから更新開始）
	vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&data));

	// 頂点データをコピー
	memcpy(data, triangleVertices, sizeof(triangleVertices));

	// マップ解除（CPUから更新終了）
	vertexBuffer->Unmap(0, nullptr);

	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress(); // バッファのGPU仮想アドレス
	vertexBufferView.StrideInBytes = sizeof(Vertex);                     // 1頂点あたりのサイズ
	vertexBufferView.SizeInBytes = sizeof(triangleVertices);             // 頂点データ全体のサイズ
}

void DrawingDelta::IndexBufferCreate(ID3D12Device* device)
{
	UINT16 traiangleIndices[] = { 0, 1, 2, }; // 三角形

	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress(); // バッファのGPU仮想アドレス
	indexBufferView.Format = DXGI_FORMAT_R16_UINT;                     // インデックスの型
	indexBufferView.SizeInBytes = sizeof(traiangleIndices);             // インデックス全体のサイズ
}