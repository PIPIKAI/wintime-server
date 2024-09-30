#include "gui.h"

#define WM_TRAYICON (WM_APP + 1)
#define GRAY_BLACK RGB(40, 40, 40)
// 假设已定义的对话框资源ID为 IDD_APIKEYDIALOG
#define IDD_APIKEYDIALOG 601
#define IDC_APIKEYEDIT 602
#define ID_EDITBOX 110
#define ID_SAVEBTN 112
#define ID_CLSBTN 113
#define ID_TEXT 114

static int SavaInputContent(TCHAR* path, TCHAR* content) {


    // FILE *fSvae ;

    // fSvae = fopen( path, "w" ) ;
    // if(fSvae == NULL)
    // {
    //     MessageBox(NULL, TEXT("文件创建失败!"), TEXT("提示"), MB_OK | MB_ICONINFORMATION) ;
    //     return -1 ;
    // }
    // fputs( content, fSvae ) ;
    // fclose(fSvae) ;
    MessageBox(NULL, TEXT("保存成功!"), TEXT("成功"), MB_OK | MB_ICONINFORMATION);

    return 0;
}
int CreateChildWindow(HWND hwnd, HWND* hwndChild, LPARAM lParam)
{
    HINSTANCE hInst = ((LPCREATESTRUCT)lParam)->hInstance;

    //创建编辑区
    hwndChild[ID_EDITBOX] = CreateWindowW(TEXT(L"edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_EDITBOX, hInst, NULL);

    //保存按钮
    hwndChild[ID_SAVEBTN] = CreateWindowW(TEXT(L"button"), TEXT(L"保存"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
        hwnd, (HMENU)ID_SAVEBTN, hInst, NULL);

    //清空按钮
    hwndChild[ID_CLSBTN] = CreateWindowW(TEXT(L"button"), TEXT(L"重置"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
        hwnd, (HMENU)ID_CLSBTN, hInst, NULL);

    return 0;
}


INT_PTR CALLBACK ApiKeyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // 初始化对话框
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            // 处理确定按钮
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            // 处理取消按钮
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



static void ShowApiKeyDialog(HINSTANCE hInst, HWND hwndParent) {
    DialogBox(hInst, MAKEINTRESOURCE(IDD_APIKEYDIALOG), hwndParent, ApiKeyDialogProc);
}


void Gui::ShowContextMenu(HWND hwnd_) {
    POINT pt;
    GetCursorPos(&pt);

    HMENU hMenu = CreatePopupMenu();
    if (hMenu) {
        InsertMenuW(hMenu, 0, MF_BYPOSITION | MF_STRING, SETTINGKEY, TEXT(L"设置api-key"));
        InsertMenuW(hMenu, 1, MF_BYPOSITION | MF_STRING, CONSOLEKEY, consleState == 0 ? TEXT(L"显示控制台") :  TEXT(L"关闭控制台"));
        InsertMenuW(hMenu, 2, MF_BYPOSITION | MF_STRING, ID_EXIT, L"退出");
        SetForegroundWindow(hwnd_);
        TrackPopupMenu(hMenu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd_, NULL);
        PostMessage(hwnd_, WM_NULL, 0, 0);

        DestroyMenu(hMenu);
    }
}

void RemoveIcon(HWND hwnd_) {
    SetClassLongPtr(hwnd_, GCLP_HICON, reinterpret_cast<LONG_PTR>(nullptr));
}

Gui::Gui(HINSTANCE hInstance, LPCSTR className, LPCSTR iconName)
    : hInstance_(hInstance), hwnd_(nullptr), className_(className), iconName_(iconName) {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    setConsoleState();
}
Gui::Gui()
{
}
Gui::~Gui() {
}

bool Gui::RegisteClass() {
    // 创建一个灰黑色的画刷
    // HBRUSH hbrGrayBlack = CreateSolidBrush(GRAY_BLACK);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance_;
    wc.lpszClassName = TEXT(className_);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    return RegisterClass(&wc);
}

bool Gui::Create(LPCSTR appName) {
    hwnd_ = CreateWindowEx(
        0,
        TEXT(className_),
        TEXT(appName),
        WS_OVERLAPPEDWINDOW,
        (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HIGHT) / 2,
        WINDOW_WIDTH, WINDOW_HIGHT,
        nullptr,
        nullptr,
        hInstance_,
        nullptr);
    return hwnd_ != nullptr;
}
bool Gui::AddIcon() {
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd_;
    nid.uID = 1001;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpy(nid.szTip, TEXT(iconName_));
    Shell_NotifyIcon(NIM_ADD, &nid);
    return true;
}

// 打开控制台
void   Gui::setConsoleState() {
    HWND consoleWindow = GetConsoleWindow();

    if (consleState) {
        if (consoleWindow == NULL) {
            AllocConsole();
            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONIN$", "r", stdin);
        }
        else {
            ShowWindow(consoleWindow, SW_SHOW);
        }
        std::cout << "显示控制台" << std::endl;

    }
    else {
        std::cout << "关闭控制台" << std::endl;
        ShowWindow(consoleWindow,   SW_HIDE);
    }
}
void Gui::Show(int nCmdShow) {
    ShowWindow(hwnd_, nCmdShow);
}

LRESULT CALLBACK Gui::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RECT    rect;
    static  HWND hwndChild[255];
    static    TCHAR* szBuffer;        //缓冲区 

    switch (uMsg) {
    case ID_EDITBOX: {

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case ID_CLSBTN:
        SetWindowText(hwndChild[ID_EDITBOX], TEXT(""));
        InvalidateRect(hwnd, NULL, FALSE);
        InvalidateRect(hwndChild[ID_EDITBOX], NULL, FALSE);
        return 0;
    case WM_TRAYICON:
        if (LOWORD(lParam) == WM_RBUTTONDOWN) {
            ShowContextMenu(hwnd);
            return 0;
        }
        else if (LOWORD(lParam) == WM_LBUTTONUP) {
            if (IsWindowVisible(hwnd)) {
                ShowWindow(hwnd, SW_HIDE);
            }
            else {
                ShowWindow(hwnd, SW_SHOW);
                SetForegroundWindow(hwnd); // 当显示窗口时，将窗口带到前台
            }
        }
        break;
    case WM_DESTROY:
        RemoveIcon(hwnd);
        delete szBuffer;
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == SETTINGKEY) {
            // 调用函数显示输入框并处理结果

            ShowWindow(hwnd, SW_SHOW);
            SetForegroundWindow(hwnd); // 当显示窗口时，将窗口带到前台
            return 0;
        }
        else if (LOWORD(wParam) == ID_EXIT) {
            DestroyWindow(hwnd);
            return 0;
        }else if (LOWORD(wParam) == CONSOLEKEY) {
            consleState = 1 - consleState;
            setConsoleState();
            return 0;
        }
        break;
        // 绘制窗口中的内容
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // // 获取当前时间
        TextOutW(hdc, 0, 0, L"api-key:", lstrlenW(L"api-key:"));
        EndPaint(hwnd, &ps);
    } break;
    case WM_CREATE: {
        CreateChildWindow(hwnd, hwndChild, lParam);
    } break;
    case WM_SIZE:
        GetClientRect(hwnd, &rect);
        MoveWindow(hwndChild[ID_EDITBOX], 51, 0, rect.right, rect.bottom - 30, TRUE);        //调整文本编辑区
        MoveWindow(hwndChild[ID_SAVEBTN], rect.right - 110, rect.bottom - 28, 50, 25, TRUE);            //调整保存按钮
        MoveWindow(hwndChild[ID_CLSBTN], rect.right - 55, rect.bottom - 28, 50, 25, TRUE);            //调整清空按钮
        return 0;
        // 创建一个定时器
        // 定时器过程（NULL表示使用窗口过程）
        // case WM_TIMER:
        //     switch (wParam) {
        //         case IDT_TIMER1:
        //             InvalidateRect(hwnd, NULL, TRUE); // 使整个窗口无效，请求重绘
        //             break;
        //     }
        //     break;
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        return 0;

    case ID_SAVEBTN: {
        int iLength = GetWindowTextLength(hwndChild[ID_EDITBOX]);
        if (iLength != 0)
            szBuffer = (TCHAR*)malloc(iLength * 2);
        else
            return -1;
        // GetWindowText( hwndChild[ID_EDITBOX], szBuffer, GetWindowTextLength(hwndChild[ID_EDITBOX]) + 1 ) ;

        if (GetWindowText(hwndChild[ID_EDITBOX], szBuffer, 256) < 1)
        {
            MessageBoxW(NULL, TEXT(L"api-key不能为空"), TEXT(L"提示"), MB_OK | MB_ICONINFORMATION);
            return -1;
        }
        SavaInputContent(szBuffer, szBuffer);
        return 0;
    }


    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}



HWND Gui::hwndInput_ = nullptr;

int Gui::consleState = 0;