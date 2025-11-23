#include "Shader.h"

#pragma comment(lib, "d3dcompiler.lib")
ID3DBlob* Shader::VertexShaderCreate()
{
	ID3D10Blob* vertexShader{};

	HRESULT hr = D3DCompileFromFile
	(
		L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"vs",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShader,
		&error
	);

	if (error != NULL)
		error->Release();
	return vertexShader;
}

ID3DBlob* Shader::PixelShaderCreate()
{
	ID3DBlob* pixelShader{};

	HRESULT hr = D3DCompileFromFile
	(
		L"PixelShader.hlsl",
		nullptr,
		nullptr,
		"ps",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShader,
		&error
	);

	if (error != NULL)
		error->Release();
	return pixelShader;
}