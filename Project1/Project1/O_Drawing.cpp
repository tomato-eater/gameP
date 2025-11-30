#include "O_Drawing.h"


//commandをリセット
void CommandReset(UINT index, ID3D12CommandAllocator* allocator, ID3D12GraphicsCommandList* list)
{
	allocator->Reset();
	list->Reset(allocator, nullptr);
}

//リソースバリア
void ResourceBarrier(ID3D12Resource* target, ID3D12GraphicsCommandList* list)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = target;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	list->ResourceBarrier(1, &barrier);
}

//レンダ―ターゲットの取得
D3D12_CPU_DESCRIPTOR_HANDLE Target(ID3D12DescriptorHeap* heap, UINT index, ID3D12Device* device)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	D3D12_DESCRIPTOR_HEAP_TYPE type = heap->GetDesc().Type;
	handle.ptr += index * device->GetDescriptorHandleIncrementSize(type);
	return handle;
}

//設定
D3D12_VIEWPORT Port(int width, int height)
{
	D3D12_VIEWPORT port = {};
	port.TopLeftX = 0.0f;
	port.TopLeftY = 0.0f;
	port.Width = width;
	port.Height = height;
	port.MinDepth = 0.0f;
	port.MaxDepth = 1.0f;
	return port;
}

//シザー
D3D12_RECT Rect(int width, int height)
{
	D3D12_RECT rect = {};
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	return rect;
}

//描画 (出力では無い)
void Painting(ID3D12GraphicsCommandList* list, D3D12_VERTEX_BUFFER_VIEW* vertex, D3D12_INDEX_BUFFER_VIEW* index)
{
	list->IASetVertexBuffers(0, 1, vertex);
	list->IASetIndexBuffer(index);
	list->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP <-四角形可   D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST<-四角形不可　　？？？
	list->DrawInstanced(4, 1, 0, 0);
}


void O_Drawing::Draw(UINT index, O_DirectX12& directX12, O_DirectXCrear& directCrear, O_PipLine& line, O_Signature& sign, int width, int height, O_Buffer& buffer)
{
	CommandReset(index, directCrear.GetAllocator(index), directCrear.GetList());

	ResourceBarrier(directCrear.GetTarget(index), directCrear.GetList());

	D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { Target(directCrear.GetHeap(), index, directX12.GetDevice()) };
	directCrear.GetList()->OMSetRenderTargets(1, handles, false, nullptr);

	float backColor[] = { 0.9f, 0.6f, 0.2f, 1.0f }; //背景色
	directCrear.GetList()->ClearRenderTargetView(handles[0], backColor, 0, nullptr);
	directCrear.GetList()->SetPipelineState(line.GetPipLine());		 //パイプラインステート
	directCrear.GetList()->SetGraphicsRootSignature(sign.GetRoot()); //ルートシグネチャー

	D3D12_VIEWPORT viePot = Port(width, height);
	directCrear.GetList()->RSSetViewports(1, &viePot);

	D3D12_RECT scissorRect = Rect(width, height);
	directCrear.GetList()->RSSetScissorRects(1, &scissorRect);

	Painting(directCrear.GetList(), buffer.GetVer(), buffer.GetInd());

	directCrear.GetList()->Close(); //コマンドリストをクローズ

	ID3D12CommandList* lists[] = { directCrear.GetList() };
	directX12.GetCommandQ()->ExecuteCommandLists(_countof(lists), lists); //画面クリアと次の画面に更新 (出力では無い)
}