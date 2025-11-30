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
	if (Vertex()) return true;

	if (Pixel())return true;

	return false;
}

//頂点シェーダーのコンパイル
bool O_Shader::Vertex()
{
	HRESULT hr = D3DCompileFromFile
	(
		L"VertexShader.hlsl",
		nullptr, nullptr,
		"vs", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &vShader, nullptr
	);

	if (FAILED(hr))
	{
		assert(false && "頂点シェーダーのコンパイルに失敗");
		return true;
	}
	return false;
}

//ピクセルシェーダーのコンパイル
bool O_Shader::Pixel()
{
	HRESULT hr = D3DCompileFromFile
	(
		L"PixelShader.hlsl",
		nullptr, nullptr,
		"ps", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pShader, nullptr
	);

	if (FAILED(hr))
	{
		assert(false && "ピクセルシェーダーのコンパイルに失敗");
		return true;
	}
	return false;
}