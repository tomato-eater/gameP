#include "PipLineStateObj.h"

ID3D12PipelineState* PipLineStateObj::PipLineObjCreate(ID3D12RootSignature* rootSignature, ID3DBlob* vertexShader, ID3DBlob* pixelShader, ID3D12Device* device)
{
	D3D12_INPUT_ELEMENT_DESC inputElementDesce[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc = {};

	psDesc.InputLayout = { inputElementDesce,_countof(inputElementDesce) };
	psDesc.pRootSignature = rootSignature;

	psDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
	psDesc.PS = { pixelShader->GetBufferPointer(),pixelShader->GetBufferSize() };

	psDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	psDesc.DepthStencilState.DepthEnable = FALSE;
	psDesc.DepthStencilState.StencilEnable = FALSE;

	psDesc.SampleMask = UINT_MAX;
	psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psDesc.NumRenderTargets = 1;
	psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psDesc.SampleDesc.Count = 1;

	ID3D12PipelineState* piplineObj;
	device->CreateGraphicsPipelineState(&psDesc, IID_PPV_ARGS(&piplineObj));

	return piplineObj;
}