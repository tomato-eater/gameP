#include<Windows.h>

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
    //wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GameWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    // 2. �E�B���h�E�쐬
    HWND hwnd=CreateWindow
    (

    )

    // 3. ���b�Z�[�W���[�v
    return 0;
}