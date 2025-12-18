#include "O_Window.h"

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

// ウィンドウ登録と作成
bool O_Window::Create(HINSTANCE hInstance, int width, int height, string name)
{
    //登録
	WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
	wc.lpszClassName = name.c_str();
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClass(&wc);

    //作成
    hwnd = CreateWindow(
        wc.lpszClassName,
        wc.lpszClassName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL,
        hInstance,
        NULL
    );

    if (!hwnd)
    {
        assert(false && "ウィンドウの作成に失敗");
        return true;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

	return false;
}