#pragma once

#include<d3d12.h>
#include<d3dx12.h>
class PipLineStateObj
{
public:
	PipLineStateObj() = default;
	~PipLineStateObj();

	ID3D12PipelineState* PipLineObjCreate(ID3D12RootSignature* rootSignature, ID3DBlob* vertexShader, ID3DBlob* pixelShader, ID3D12Device* device);
};

