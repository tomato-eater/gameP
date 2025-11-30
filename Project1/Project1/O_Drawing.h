#pragma once
#include <cassert>

#include "DirectX12.h"
#include "O_DirectXCrear.h"
#include "O_PipLine.h"
#include "O_Signature.h"
#include "O_Buffer.h"

class O_Drawing
{
private:

public:
	O_Drawing() = default;
	~O_Drawing() = default;

	void Draw(UINT index, O_DirectX12& directX12, O_DirectXCrear& directCrear, O_PipLine& line, O_Signature& sign, int width, int height, O_Buffer& buffer);
};

