#pragma once
#include <cassert>

#include <DirectXMath.h>

#include "O_DirectX12.h"

/// <summary>
/// バッファー
/// </summary>
class O_Buffer
{
private:
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

public:
	O_Buffer() = default;
	~O_Buffer() = default;

	[[nodiscard]] bool Create(O_DirectX12& directX);

	bool CreateVertex(ID3D12Device* device);
	bool CreateIndex(ID3D12Device* device);

	D3D12_VERTEX_BUFFER_VIEW* GetVer() { return &vertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW* GetInd() { return &indexBufferView; }

};

