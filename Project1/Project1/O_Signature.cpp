#include "O_Signature.h"

O_Signature::~O_Signature()
{
	if (rootSignature)
	{
		rootSignature->Release();
		rootSignature = nullptr;
	}
}

bool O_Signature::Create(O_DirectX12& direct)
{
	D3D12_DESCRIPTOR_RANGE r0;
	r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	r0.NumDescriptors = 1;
	r0.BaseShaderRegister = 0;
	r0.RegisterSpace = 0;
	r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE r1;
	r1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	r1.NumDescriptors = 1;
	r1.BaseShaderRegister = 1;
	r1.RegisterSpace = 0;
	r1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rPara[2]{};
	rPara[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rPara[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rPara[0].DescriptorTable.NumDescriptorRanges = 1;
	rPara[0].DescriptorTable.pDescriptorRanges = &r0;

	rPara[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rPara[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rPara[1].DescriptorTable.NumDescriptorRanges = 1;
	rPara[1].DescriptorTable.pDescriptorRanges = &r1;

	D3D12_ROOT_SIGNATURE_DESC rootDesc{};
	rootDesc.NumParameters = 2;
	rootDesc.pParameters = rPara;
	rootDesc.NumStaticSamplers = 0;
	rootDesc.pStaticSamplers = nullptr;
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3D10Blob* sign;
	ID3D10Blob* error{};
	HRESULT hr = D3D12SerializeRootSignature
	(
		&rootDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&sign,
		&error
	);

	bool su = SUCCEEDED(hr);
	if (!su)
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ルートシグネチャーのシリアライズに失敗");
	}
	else
	{
		hr = direct.GetDevice()->CreateRootSignature
		(
			0,
			sign->GetBufferPointer(),
			sign->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature)
		);
		su &= SUCCEEDED(hr);
		if (!su)
			assert(false && "ルートシグネチャーの生成に失敗");
	}
	if (error)error->Release();
	if (sign)sign->Release();

	return !su;
}