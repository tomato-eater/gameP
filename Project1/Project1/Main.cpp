#include<Windows.h>
#include "DirectX12.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        //��ʍX�V����
        return 0;

    case WM_KEYDOWN:
        //�L�[���͏���
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain
(
    HINSTANCE hInstance,      // �A�v���P�[�V�����̎��ʔԍ�
    HINSTANCE hPrevInstance,  // ��{�g��Ȃ��Ă���
    LPSTR lpCmdLine,          // �R�}���h���C�������i�N�����̃I�v�V�����j
    int nCmdShow              // �E�B���h�E�̕\�����@�i�ő剻�A�ŏ����Ȃǁj
)
{   // �����Ƀ��C���̏���������
    // 1. �E�B���h�E�N���X�o�^
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GameWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    // 2. �E�B���h�E�쐬
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

    // 3. ���b�Z�[�W���[�v
    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}