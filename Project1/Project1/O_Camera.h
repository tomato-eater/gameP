#pragma once

#include <DirectXMath.h>
#include <cmath>
using namespace DirectX;

class O_Camera
{
private:
	XMMATRIX view{};
	XMMATRIX projection{};

	XMFLOAT3 position{};
	XMFLOAT3 target{};
	XMFLOAT3 up{};

public:

	struct ConstBufferData
	{
		XMMATRIX view{};
		XMMATRIX projection{};
	};

	O_Camera() = default;
	~O_Camera() = default;

	void Instalize(int width, int height);
	void Updata();

	XMMATRIX GetView() { return view; }
	XMMATRIX GetProj() { return projection; }
};

