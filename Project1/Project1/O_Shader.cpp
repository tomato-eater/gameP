#include "O_Shader.h"

O_Shader::~O_Shader()
{
	if (vShader)
	{
		vShader->Release();
		vShader = nullptr;
	}
	if (pShader)
	{
		pShader->Release();
		pShader = nullptr;
	}
}

bool O_Shader::Create()
{
	ID3DBlob* error{};
	HRESULT hr = D3DCompileFromFile
	(
		L"shader.hlsl",
		nullptr, nullptr,
		"vs", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &vShader, &error
	);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダーのコンパイルに失敗");
	}

	hr = D3DCompileFromFile
	(
		L"shader.hlsl",
		nullptr, nullptr,
		"ps", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pShader, &error
	);
	if (FAILED(hr))
	{
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダーのコンパイルに失敗");
	}

	if (error)error->Release();

	return false;
}