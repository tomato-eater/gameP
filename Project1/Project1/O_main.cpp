#include "O_Window.h"
#include "O_DirectX12.h"
#include "O_Heap.h"
#include "O_RenTarget.h"
#include "O_Allocator.h"
#include "O_List.h"
#include "O_Fence.h"
#include "O_DeltaPolygon.h"
#include "O_Signature.h"
#include "O_Shader.h"
#include "O_PipLine.h"
#include "O_Camera.h"
#include "O_ConstantBuffer.h"
#include "O_Obj.h"
#include "O_ViewRect.h"

class O_Operations
{
private:
	pair<int, int> size = { 800, 600 };
	string name = "Orange";

	O_Window wind{};				 //ウィンドウの登録、生成
	O_DirectX12 directX12{};		 //Factory Adapter Evice CommandQueue swapChain
	O_Heap descriptorHeap{};		 //ディスクリプタヒープ
	O_RenTarget target{};			 //レンダ―ターゲット
	O_Allocator allocator[2]{};		 //コマンドアロケーター
	O_List list{};					 //コマンドリスト
	O_Fence fence{};				 //フェンス
	O_DeltaPolygon dPoly{};			 //ポリゴンの登録
	O_Signature signature{};		 //ルートシグネチャー
	O_Shader shader{};			     //シェーダー
	O_PipLine pipLine{};			 //パイプライン

	O_Camera camera{};				 //カメラ
	O_Heap cbdHeap{};				 //ディスクリプタヒープ
	O_ConstantBuffer cameeraBuffer{};//カメラバッファー
	O_ConstantBuffer polygonBuffer{};//ポリゴンバッファー

	O_Obj polyObj{};				 //オブジェ

	O_ViewRect vr{};				 //ビューポート　シザー

public:
	O_Operations() {};
	~O_Operations() {};

	void Initialize(HINSTANCE hInstance)
	{
		if (S_OK != wind.Create(hInstance, size.first, size.second, name))
			return;

		if (directX12.Create(size.first, size.second, wind))
			return;

		if (descriptorHeap.CreateHeap(directX12, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, directX12.GetSwapChain()->GetCurrentBackBufferIndex(), false))
			return;

		if (target.CreateTarget(descriptorHeap, directX12))
			return;

		for (auto all : allocator)
			if (all.CreateAllocator(directX12))
				return;

		if (list.CreateList(directX12, allocator[0]))
			return;

		if (fence.Create(directX12))
			return;

		if (dPoly.CreateBuffers(directX12))
			return;

		if (signature.Create(directX12))
			return;

		if (shader.Create())
			return;

		if (pipLine.Create(signature, shader, directX12))
			return;

		camera.Instalize(size.first, size.second);

		if (cbdHeap.CreateHeap(directX12, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2, true))
			return;

		if (cameeraBuffer.CreateBuffer(directX12, cbdHeap, sizeof(O_Camera::ConstBufferData), 0))
			return;

		if (polygonBuffer.CreateBuffer(directX12, cbdHeap, sizeof(O_DeltaPolygon::ConstBufferData), 1))
			return;
	};
	
	void Loop()
	{
		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			camera.Updata();
			polyObj.Updata();

			const UINT bufferIndex = directX12.GetSwapChain()->GetCurrentBackBufferIndex();
			fence.Wait(bufferIndex);

			allocator[bufferIndex].ResetAllocator();
			list.ResetList(allocator[bufferIndex].GetAllocator());
			list.SetBarriert(target, bufferIndex, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { target.GetHandle(bufferIndex, descriptorHeap, directX12) };
			list.GetList()->OMSetRenderTargets(1, handles, false, nullptr);

			float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
			list.GetList()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

			list.GetList()->SetPipelineState(pipLine.GetPipLine());
			list.GetList()->SetGraphicsRootSignature(signature.GetRoot());

			vr.Set(size.first, size.second, list);

			ID3D12DescriptorHeap* p[] = { cbdHeap.GetHeap() };
			list.GetList()->SetDescriptorHeaps(1, p);
			//////
			O_Camera::ConstBufferData cameraData
			{
				XMMatrixTranspose(camera.GetView()),
				XMMatrixTranspose(camera.GetProj())
			};
			UINT8* pCameraData{};
			cameeraBuffer.GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pCameraData));
			memcpy_s(pCameraData, sizeof(cameraData), &cameraData, sizeof(cameraData));
			cameeraBuffer.GetBuffer()->Unmap(0, nullptr);
			list.GetList()->SetGraphicsRootDescriptorTable(0, cameeraBuffer.GetHandle());

			O_DeltaPolygon::ConstBufferData polyData
			{
				XMMatrixTranspose(polyObj.GetWorld()),
				polyObj.GetColor()
			};
			UINT8* pPolyData{};
			polygonBuffer.GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&polyData));
			memcpy_s(pPolyData, sizeof(polyData), &polyData, sizeof(polyData));
			polygonBuffer.GetBuffer()->Unmap(0, nullptr);
			list.GetList()->SetGraphicsRootDescriptorTable(1, polygonBuffer.GetHandle());
			////////
			dPoly.Draw(list);

			list.SetBarriert(target, bufferIndex, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			list.GetList()->Close();

			ID3D12CommandList* ppCLists[] = { list.GetList() };
			directX12.GetCommandQ()->ExecuteCommandLists(_countof(ppCLists), ppCLists);

			fence.Present(bufferIndex, directX12);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	O_Operations ops;
	ops.Initialize(hInstance);
	ops.Loop();

	return 0;
}