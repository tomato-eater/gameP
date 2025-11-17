#include<Windows.h>
#include "DirectX12.h"
#include "DirectClear.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        //画面更新処理
        return 0;

    case WM_KEYDOWN:
        //キー入力処理
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain
(
    HINSTANCE hInstance,      // アプリケーションの識別番号
    HINSTANCE hPrevInstance,  // 基本使わなくていい
    LPSTR lpCmdLine,          // コマンドライン引数（起動時のオプション）
    int nCmdShow              // ウィンドウの表示方法（最大化、最小化など）
)
{   // ここにメインの処理を書く
    // 1. ウィンドウクラス登録
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GameWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    // 2. ウィンドウ作成
    HWND hwnd = CreateWindow
    (
        "GameWindow",
        "My Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL,
        hInstance,
        NULL
    );
    ShowWindow(hwnd, nCmdShow);

    DirectX12 DX12;
    IDXGIFactory4* factory = DX12.CreateDXGIFactory();
    IDXGIAdapter1* adapter = DX12.GetHardwareAdapter(factory);
    ID3D12Device* device = DX12.CreateD3D12Device(adapter);
    ID3D12CommandQueue* commandQueue = DX12.CreateCommandQueue(device);
    IDXGISwapChain3* swapChain = DX12.CreateSwapChain(factory, commandQueue, hwnd);
	//DX12.EnableDebugLayer();　//こいつのせいで rtvHeapが作れない?

	DirectClear crear;
    ID3D12DescriptorHeap* rtvHeap = crear.DiscripterHeapDesc(device);
    UINT rtvDescriptorSize = crear.DiscripterHeapAccess(device, rtvHeap);
    ID3D12Resource* renderTargets = crear.RenderTarget(swapChain, rtvHeap, rtvDescriptorSize, device);
    ID3D12CommandAllocator* commandAllocators = crear.CommandAllocatorCreat(device);
    ID3D12GraphicsCommandList* commandList = crear.CommandList(device, commandAllocators);
	crear.CommandQueue(commandQueue, commandList);
    ID3D12Fence* fence = crear.FenceCreat(device);

    // 3. メッセージループ
    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        const UINT64 fenceValue = swapChain->GetCurrentBackBufferIndex();
		crear.FenceCheck(fenceValue, commandQueue, fence);
        crear.CommandAllocatorReset(swapChain, &commandAllocators, commandList, &renderTargets, rtvHeap, rtvDescriptorSize);

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}