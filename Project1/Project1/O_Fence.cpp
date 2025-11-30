#include "O_Fence.h"

O_Fence::~O_Fence()
{
	if (fence)
	{
		fence->Release();
		fence = nullptr;
	}
}

//フェンスの生成
bool O_Fence::Create(ID3D12Device* device)
{
	HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr))
	{
			assert(false && "フェンスの生成に失敗");
			return true;
	}

	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (fenceEvent) return false; //正常ならここを通る

	assert(false && "フェンスイベントの生成に失敗");
	return true;
}

void O_Fence::Wait(UINT index)
{
	if (fence->GetCompletedValue() < fenceValue[index]) //GPU処理が終わっていない
	{
		fence->SetEventOnCompletion(fenceValue[index], fenceEvent); //終わったら実行
		WaitForSingleObject(fenceEvent, INFINITE); //終わるまでCPUに待機命令
	}
}

void O_Fence::Present(UINT index, O_DirectX12& directX)
{
	fenceValue[index] = ++nextValue;
	directX.GetCommandQ()->Signal(fence, fenceValue[index]);
	directX.GetSwapChain()->Present(1, 0); //出力
}