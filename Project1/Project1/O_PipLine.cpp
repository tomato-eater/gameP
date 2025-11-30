#include "O_PipLine.h"

O_PipLine::~O_PipLine()
{
	if (pipLine)
	{
		pipLine->Release();
		pipLine = nullptr;
	}
}

bool O_PipLine::Create(O_DirectX12& directX, O_Signature& signature, O_Shader& shader)
{
	if (CreatePipLine(directX.GetDevice(), signature.GetRoot(), shader.GetVS(), shader.GetPS())) return true;

	return false;
}

bool O_PipLine::CreatePipLine(ID3D12Device* device, ID3D12RootSignature* signature, ID3D10Blob* vs, ID3D10Blob* ps)
{
	D3D12_INPUT_ELEMENT_DESC IEdesc[] =
	{
		{
			"POSITION", 0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 0,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR", 0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 12,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	D3D12_RASTERIZER_DESC rasDesc{};
	rasDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasDesc.FrontCounterClockwise = false;
	rasDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasDesc.SlopeScaledDepthBias = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasDesc.DepthClipEnable = true;
	rasDesc.MultisampleEnable = false;
	rasDesc.AntialiasedLineEnable = false;
	rasDesc.ForcedSampleCount = 0;
	rasDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	const D3D12_RENDER_TARGET_BLEND_DESC RTBdesc =
	{
		FALSE, FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO,	D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO,	D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL
	};

	D3D12_BLEND_DESC Bdesc{};
	Bdesc.AlphaToCoverageEnable = false;
	Bdesc.IndependentBlendEnable = false;
	for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		Bdesc.RenderTarget[i] = RTBdesc;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSdesc = {};
	GPSdesc.InputLayout = { IEdesc,_countof(IEdesc) };
	GPSdesc.pRootSignature = signature;
	GPSdesc.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
	GPSdesc.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };
	GPSdesc.RasterizerState = rasDesc;
	GPSdesc.BlendState = Bdesc;
	GPSdesc.DepthStencilState.DepthEnable = FALSE;
	GPSdesc.DepthStencilState.StencilEnable = FALSE;
	GPSdesc.SampleMask = UINT_MAX;
	GPSdesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	GPSdesc.NumRenderTargets = 1;
	GPSdesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	GPSdesc.SampleDesc.Count = 1;

	HRESULT hr = device->CreateGraphicsPipelineState(&GPSdesc, IID_PPV_ARGS(&pipLine));
	if (FAILED(hr))
	{
		assert(false && "パイプラインステートオブジェの生成に失敗");
		return true;
	}

	return false;
}