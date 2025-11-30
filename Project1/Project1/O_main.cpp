#include <cassert>
#include "O_Window.h"
#include "O_DirectX12.h"
#include "O_DirectXCrear.h"
#include "O_Fence.h"
#include "O_Buffer.h"
#include "O_Signature.h"
#include "O_Shader.h"
#include "O_PipLine.h"
#include "O_Drawing.h"

// 作成したり、使用したりする操作をまとめたクラス
class Operations
{
private:
	pair<int, int> size = { 800,600 };
	string name = "Orange";
	
	O_Window window{};			 //ウィンドウの登録、生成
	O_DirectX12 directX12{};		 //Factory Adapter Evice CommandQueue wapChain
	O_DirectXCrear directCrear{};	 //Heap RenderTarget CommandAllocator CommandList 
	O_Fence fence{};				 //Fence FenceEvent
	O_Buffer buffer{};			 //頂点バッファ　指数バッファ
	O_Signature signature{};		 //ルートシグネチャー	
	O_Shader shader{};			 //シェーダーのコンパイル
	O_PipLine pipLine{};			 //パイプラインオブジェ
	O_Drawing draw{};

public:
	Operations() = default;
	~Operations() = default;

	void Initialize(HINSTANCE hInstance)
	{
		if (S_OK != window.Create(hInstance, size.first, size.second, name))
		{
			assert(false && "ウィンドウの作成に失敗");
			return;
		}

		if (directX12.Create(size.first, size.second, window))	 return;

		if (directCrear.Create(directX12))						 return;

		if (fence.Create(directX12.GetDevice()))				 return;

		if (buffer.Create(directX12))							 return;

		if (signature.Create(directX12))						 return;

		if (shader.Create())									 return;

		if (pipLine.Create(directX12, signature, shader))		 return;
	}

	void Loop() //メッセージループ
	{
		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			const UINT bufferIndex = directX12.GetSwapChain()->GetCurrentBackBufferIndex();
			fence.Wait(bufferIndex);

			draw.Draw(bufferIndex, directX12, directCrear, pipLine, signature, size.first, size.second, buffer);

			fence.Present(bufferIndex, directX12);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Operations ops;
	ops.Initialize(hInstance);
	ops.Loop();

	return 0;
}