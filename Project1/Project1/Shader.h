#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>
class Shader
{
private:
	ID3D10Blob* error{};

public:
	Shader() {};
	~Shader() {};

	ID3DBlob* VertexShaderCreate();
	ID3DBlob* PixelShaderCreate();
};

