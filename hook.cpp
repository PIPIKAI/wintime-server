#include "hook.h"
// �¼��ص�����
void CALLBACK WinEventProc(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    // ֻ��עǰ̨���ڱ仯
    if (event == EVENT_SYSTEM_FOREGROUND) {
        // ��ȡ����ID
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);

        // ��ȡ���ڱ���
        const int bufferSize = 1024;
        char windowTitle[bufferSize];
        GetWindowText(hwnd, windowTitle, bufferSize);
        RECT windowsRect;
        GetWindowRect(hwnd, &windowsRect);
        // ��ȡ��ǰʱ��
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_c);

        // ���Դ򿪽����Բ�ѯ������
        char processName[MAX_PATH] = { 0 };
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess) {
            //WCHAR processName[MAX_PATH] = L"<unknown>";
            DWORD bufSize = sizeof(processName) / sizeof(WCHAR);
            // ��ȡ���̵Ŀ�ִ���ļ�·��
            if (QueryFullProcessImageName(hProcess, 0, processName, &bufSize)) {
                // processName ���ڰ����˽��̵�����·�������Դ�����ȡ�ļ���
                CHAR* fileName = strrchr(processName, '\\');

                if (fileName) {
                    fileName += 1; // �ƶ�����б�ܺ�ĵ�һ���ַ�����ʵ�ʵ��ļ�����ʼλ��
                }
                else {
                    fileName = processName; // ���δ�ҵ���б�ܣ������ַ��������ļ���
                }
                //std::wcout << std::put_time(&now_tm, L"%Y-%m-%d %H:%M:%S") << TEXT(": �������ƣ�") << fileName
                    //<< TEXT(", ����PID: ") << pid << TEXT(", ���ڱ���: ") << windowTitle << std::endl;

                if (0 != strlen(windowTitle)) {
                    std::cout << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "|��������"<< fileName <<" | PID:"<< pid<<"|"<<"����title:"<<windowTitle << std::endl;
                }
            }

        }
    }
}

// ����¼��Ļص�����
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
        switch (wParam) {
        case WM_LBUTTONUP: {
            //std::wcout << L"LEFT CLICK UP";
        } break;
        case WM_LBUTTONDOWN: {
            std::cout << "LEFT CLICK DOWN"<<std::endl;
        } break;
        case WM_RBUTTONUP: {
            //std::wcout << L"RIGHT CLICK UP";
        } break;
        case WM_RBUTTONDOWN: {
            //std::wcout << L"RIGHT CLICK DOWN";
        } break;
        case WM_CHAR: {
            //std::wcout << (TCHAR)wParam;
        } break;
        }
        if (wParam) {
            // std::wcout << L" at (" << p->pt.x << ", " << p->pt.y << ")" << std::endl;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        if (wParam == WM_KEYDOWN) {
            std::wcout << L"Key Down: " << p->vkCode << std::endl;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}




void watchwindows() {
    // ��װ�¼����ӣ�����ǰ̨���ڱ仯
    HWINEVENTHOOK windowHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        NULL, WinEventProc,
        0, 0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    HHOOK keyboardInputHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    HHOOK mouseInputHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);

    // ��Ϣѭ����ȷ������������У������¼�
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        //std::wcout << L"get message" << std::endl;
        DispatchMessage(&msg);
    }

    // �Ƴ�����
    UnhookWinEvent(windowHook);
    UnhookWindowsHookEx(keyboardInputHook);
    UnhookWindowsHookEx(mouseInputHook);
}