#pragma once
#include <cassert>

#include <d3d12.h>
#include <D3Dcompiler.h>
class O_Shader
{
private:
	ID3DBlob* vShader;
	ID3DBlob* pShader;

public:
	O_Shader() = default;
	~O_Shader();

	[[nodiscard]] bool Create();

	bool Vertex();
	bool Pixel();

	ID3D10Blob* GetVS() { return vShader; }
	ID3D10Blob* GetPS() { return pShader; }
};

