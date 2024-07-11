#include <windows.h>

#define IDD_DIALOG1 101
// ȫ�ֱ���
HINSTANCE hInst; // ����ʵ�����
NOTIFYICONDATA nid; // ����ͼ������
HWND hWnd; // �����ھ��

// ��������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ShowContextMenu(HWND hWnd);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // ע�ᴰ����
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("TrayApp");
    RegisterClass(&wc);

    // �������ش���
    hWnd = CreateWindow(wc.lpszClassName, TEXT("Tray Application"), WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    // ��ʼ������ͼ��
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    lstrcpy(nid.szTip, TEXT("Tray Application"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    // ��Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // ��������ͼ��
    Shell_NotifyIcon(NIM_DELETE, &nid);

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_USER + 1:
            if (lParam == WM_RBUTTONUP) {
                ShowContextMenu(hWnd);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1: // ����
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
                    break;
                case 2: // �˳�
                    PostQuitMessage(0);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void ShowContextMenu(HWND hWnd) {
    POINT pt;
    GetCursorPos(&pt);
    HMENU hMenu = CreatePopupMenu();
    InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, 1, TEXT("����"));
    InsertMenu(hMenu, 1, MF_BYPOSITION | MF_STRING, 2, TEXT("�˳�"));
    SetForegroundWindow(hWnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
