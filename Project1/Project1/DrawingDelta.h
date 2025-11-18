#pragma once

#include <d3d12.h>
class DrawingDelta
{
private:
	ID3D12Resource* vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ID3D12Resource* indexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

public:
	DrawingDelta() {}
	~DrawingDelta() {}

	void TriangleVertices(ID3D12Device* device);
	void IndexBufferCreate(ID3D12Device* device);

};

