#pragma once

#include<d3d12.h>
class DeltaBuffer
{
public:
	DeltaBuffer() {}
	~DeltaBuffer() {}

	D3D12_VERTEX_BUFFER_VIEW VertexBufferCreate(ID3D12Device* device);
	D3D12_INDEX_BUFFER_VIEW IndexBufferCreate(ID3D12Device* device);

};