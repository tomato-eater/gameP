#pragma once

#include <DirectXMath.h>
#include <cmath>
using namespace DirectX;

class O_Obj
{
private:
	XMMATRIX world = XMMatrixIdentity();
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	float move{};

public:
	struct ConstantBufferData
	{
		XMMATRIX world{};
		XMFLOAT4 color{};
	};

	O_Obj() = default;
	~O_Obj() = default;

	void Updata();

	XMMATRIX GetWorld() { return world; }
	XMFLOAT4 GetColor() { return color; }
};

