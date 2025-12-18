#pragma once

#include "O_DirectX12.h"
#include "O_List.h"

#include <DirectXMath.h>
using namespace DirectX;

class O_DeltaPolygon
{
private:
	ID3D12Resource* verBuffer{};
	ID3D12Resource* indBuffer{};

	D3D12_VERTEX_BUFFER_VIEW verView{};
	D3D12_INDEX_BUFFER_VIEW indView{};

	bool CreateVertex(ID3D12Device* device);
	bool CreateIndex(ID3D12Device* device);

public:
	O_DeltaPolygon() = default;
	~O_DeltaPolygon();

	struct ConstBufferData
	{
		XMMATRIX world{};
		XMFLOAT4 color{};
	};

	[[nodiscard]] bool CreateBuffers(O_DirectX12& direct);

	void Draw(O_List& list);
};

