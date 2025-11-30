#include "O_Signature.h"

O_Signature::~O_Signature()
{
	if (rootSignature)
	{
		rootSignature->Release();
		rootSignature = nullptr;
	}
}

bool O_Signature::Create(O_DirectX12& directX)
{
	if (CreateSignature(directX.GetDevice())) return true;

	return false;
}

//ルートシグネチャーの生成
bool O_Signature::CreateSignature(ID3D12Device* device)
{
	D3D12_ROOT_SIGNATURE_DESC rootDesc{};
	rootDesc.NumParameters = 0;
	rootDesc.pParameters = nullptr;
	rootDesc.NumStaticSamplers = 0;
	rootDesc.pStaticSamplers = nullptr;
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3D10Blob* sign;
	HRESULT hr = D3D12SerializeRootSignature
	(
		&rootDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&sign,
		nullptr
	);
	if (FAILED(hr))
	{
		assert(false && "ルートシグネチャーのシリアライズに失敗");
		return true;
	}

	hr = device->CreateRootSignature
	(
		0,
		sign->GetBufferPointer(),
		sign->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);
	if (FAILED(hr))
	{
		assert(false && "ルートシグネチャーの生成に失敗");
		return true;
	}

	sign->Release();

	return false;
}