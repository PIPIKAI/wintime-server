#include "hook.h"
// 事件回调函数

static int filterFileName(const char * fileName) {

    for (const char* s : filters) {
        if (strcmp(s, fileName) == 0) {
            return 0;
        }
    }
    return 1;
}
void deepCopy(char*& destination, const char* source) {
    if (source == nullptr) {
        destination = nullptr;
    }
    else {
        // Allocate memory for the destination string, including space for the null terminator
        destination = new char[strlen(source) + 1];
        // Copy the source string to the destination string, including the null terminator
        strcpy(destination, source);
        source;
    }
}

char* getAppDataFolderPath() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        return path;
    }
    else {
        std::cerr << "Failed to get AppData folder path." << std::endl;
        return nullptr;
    }
}

bool createFolder(const char * folderPath) {
    if (CreateDirectoryA(folderPath, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        return true;
    }
    else {
        std::cerr << "Failed to create folder: " << folderPath << std::endl;
        return false;
    }
}

void CALLBACK WinEventProc(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    // 只关注前台窗口变化
    if (event == EVENT_SYSTEM_FOREGROUND) {
        // 获取进程ID
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);

        // 获取窗口标题
        const int bufferSize = 1024;
        char windowTitle[bufferSize];
        GetWindowText(hwnd, windowTitle, bufferSize);
        RECT windowsRect;
        GetWindowRect(hwnd, &windowsRect);
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_c);
        long long  now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        // 尝试打开进程以查询其名称
        char processName[MAX_PATH] = { 0 };
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (hProcess) {
            //WCHAR processName[MAX_PATH] = L"<unknown>";
            DWORD bufSize = sizeof(processName) / sizeof(WCHAR);
            // 获取进程的可执行文件路径
            if (QueryFullProcessImageName(hProcess, 0, processName, &bufSize)) {
                // processName 现在包含了进程的完整路径，可以从中提取文件名
                CHAR* fileName = strrchr(processName, '\\');

                if (fileName) {
                    fileName += 1; // 移动到反斜杠后的第一个字符，即实际的文件名开始位置
                }
                else {
                    fileName = processName; // 如果未找到反斜杠，整个字符串就是文件名
                }

                if (!filterFileName(fileName)) {
                    return;
                }

                long long spend = now_seconds - last_timestamp;
                Information now_info = { 1,fileName ,windowTitle ,now_seconds };

                if (last_info.uid == 0 || strcmp(last_info.windows_title, now_info.windows_title) != 0) {

                    last_info.duration = spend;
                    if (last_info.uid != 0) {
                        std::cout << "[OLD]" << " | Windows_title: " << last_info.windows_title << " | duration: " << last_info.duration << "|" << std::endl;
                        history.push(last_info);
                    }
                    std::cout << "[NOW] " << " | Windows_title: " << now_info.windows_title << std::endl;

                    last_timestamp = now_seconds;
                    last_info.created_time = now_info.created_time;
                    deepCopy(last_info.process_name, now_info.process_name);
                    deepCopy(last_info.windows_title, now_info.windows_title);
                    last_info.uid = now_info.uid;
                }
            }

        }
    }
}

// 鼠标事件的回调函数
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




MSG watchwindows() {
    // 安装事件钩子，监听前台窗口变化
    HWINEVENTHOOK windowHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        NULL, WinEventProc,
        0, 0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    //HHOOK keyboardInputHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    //HHOOK mouseInputHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);

    // 消息循环，确保程序持续运行，监听事件
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        //std::wcout << L"get message" << std::endl;
        DispatchMessage(&msg);
    }

    // 移除钩子
    UnhookWinEvent(windowHook);
    //UnhookWindowsHookEx(keyboardInputHook);
    //UnhookWindowsHookEx(mouseInputHook);

    return msg;
}
