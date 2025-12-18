#include "O_Allocator.h"

O_Allocator::~O_Allocator()
{
	if (commandAllocator)
	{
		commandAllocator->Release();
		commandAllocator = nullptr;
	}
}

bool O_Allocator::CreateAllocator(O_DirectX12& direct)
{
		HRESULT hr = direct.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
		if (FAILED(hr))
		{
			assert(false && "コマンドアロケータの生成に失敗");
			return true;
		}
	
	return false;
}