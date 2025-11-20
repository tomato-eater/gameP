#include "RootSignature.h"

ID3D12RootSignature* RootSignature::RootSignatureCreate(ID3D12Device* device)
{
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pStaticSamplers = nullptr;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3D10Blob* signature;
	ID3D10Blob* error;
	D3D12SerializeRootSignature
	(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		&error
	);
	if (error)
		error->Release();

	ID3D12RootSignature* rootSignature;
	device->CreateRootSignature
	(
		0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature)
	);

	signature->Release();
	return rootSignature;
}