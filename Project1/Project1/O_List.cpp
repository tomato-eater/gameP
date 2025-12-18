#include "O_List.h"

O_List::~O_List()
{
	if (commandList)
	{
		commandList->Release();
		commandList = nullptr;
	}
}

bool O_List::CreateList(O_DirectX12& direct, O_Allocator& allo)
{
	HRESULT hr = direct.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allo.GetAllocator(), nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(hr))
	{
		assert(false && "コマンドリストの生成に失敗");
		return true;
	}

	commandList->Close();

	return false;
}

void O_List::SetBarriert(O_RenTarget& target, UINT idx, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = target.GetTarget(idx);
	barrier.Transition.StateBefore = from;
	barrier.Transition.StateAfter = to;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList->ResourceBarrier(1, &barrier);
}