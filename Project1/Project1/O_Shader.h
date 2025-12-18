#pragma once
#include <cassert>

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

	ID3D10Blob* GetVS() { return vShader; }
	ID3D10Blob* GetPS() { return pShader; }
};

