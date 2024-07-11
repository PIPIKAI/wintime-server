#include "gui.h"

#define WM_TRAYICON (WM_APP + 1)
#define GRAY_BLACK RGB(40, 40, 40)
// �����Ѷ���ĶԻ�����ԴIDΪ IDD_APIKEYDIALOG
#define IDD_APIKEYDIALOG 601
#define IDC_APIKEYEDIT 602
#define ID_EDITBOX 110
#define ID_SAVEBTN 112
#define ID_CLSBTN 113
#define ID_TEXT 114

int SavaInputContent( TCHAR *path, TCHAR *content ){


    // FILE *fSvae ;

    // fSvae = fopen( path, "w" ) ;
    // if(fSvae == NULL)
    // {
    //     MessageBox(NULL, TEXT("�ļ�����ʧ��!"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION) ;
    //     return -1 ;
    // }
    // fputs( content, fSvae ) ;
    // fclose(fSvae) ;
    MessageBox(NULL, TEXT("����ɹ�!"), TEXT("�ɹ�"), MB_OK | MB_ICONINFORMATION) ;

    return 0;
}
int CreateChildWindow(HWND hwnd, HWND *hwndChild, LPARAM lParam)
{
    HINSTANCE hInst = ((LPCREATESTRUCT) lParam) -> hInstance ;

    //�����༭��
    hwndChild[ID_EDITBOX] = CreateWindow( TEXT("edit"), NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT  ,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_EDITBOX, hInst, NULL ) ;

    //���水ť
    hwndChild[ID_SAVEBTN] = CreateWindow( TEXT("button"), TEXT("����"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
        hwnd, (HMENU)ID_SAVEBTN, hInst, NULL) ;

    //��հ�ť
    hwndChild[ID_CLSBTN] = CreateWindow( TEXT("button"), TEXT("����"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
        hwnd, (HMENU)ID_CLSBTN, hInst, NULL) ;
    
    return 0 ;
}


INT_PTR CALLBACK ApiKeyDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            // ��ʼ���Ի���
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                // ����ȷ����ť
                EndDialog(hDlg, IDOK);
                return (INT_PTR)TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) {
                // ����ȡ����ť
                EndDialog(hDlg, IDCANCEL);
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}



static void ShowApiKeyDialog(HINSTANCE hInst,HWND hwndParent) {
    DialogBox(hInst, MAKEINTRESOURCE(IDD_APIKEYDIALOG), hwndParent, ApiKeyDialogProc);
}


void ShowContextMenu(HWND hwnd_) {
    POINT pt;
    GetCursorPos(&pt);

    HMENU hMenu = CreatePopupMenu();
    if (hMenu) {
        InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, SETTINGKEY, TEXT("����api-key"));
        InsertMenu(hMenu, 1, MF_BYPOSITION | MF_STRING, ID_EXIT, TEXT("�˳�"));
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
}
Gui::~Gui() {
}

bool Gui::RegisteClass() {
    // ����һ���Һ�ɫ�Ļ�ˢ
    // HBRUSH hbrGrayBlack = CreateSolidBrush(GRAY_BLACK);

    WNDCLASS wc = {0};
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

void Gui::Show(int nCmdShow) {
    ShowWindow(hwnd_, nCmdShow);
}

LRESULT CALLBACK Gui::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RECT    rect ;
    static  HWND hwndChild[5] ;
    static    TCHAR *szBuffer ;        //������ 

    switch (uMsg) {
        case ID_EDITBOX:{
                
            InvalidateRect(hwnd, NULL, FALSE) ;
            return 0 ;
        }
        case ID_CLSBTN:
            SetWindowText( hwndChild[ID_EDITBOX], TEXT("") ) ;
            InvalidateRect(hwnd, NULL, FALSE) ;
            InvalidateRect(hwndChild[ID_EDITBOX], NULL, FALSE) ;
            return 0 ;
        case WM_TRAYICON:
            if (LOWORD(lParam) == WM_RBUTTONDOWN) {
                ShowContextMenu(hwnd);
                return 0;
            } else if (LOWORD(lParam) == WM_LBUTTONUP) {
                if (IsWindowVisible(hwnd)) {
                    ShowWindow(hwnd, SW_HIDE);
                } else {
                    ShowWindow(hwnd, SW_SHOW);
                    SetForegroundWindow(hwnd); // ����ʾ����ʱ�������ڴ���ǰ̨
                }
            }
            break;
        case WM_DESTROY:
            RemoveIcon(hwnd);
            delete szBuffer ;
            PostQuitMessage(0);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == SETTINGKEY) {
                // ���ú�����ʾ����򲢴�����

                ShowWindow(hwnd, SW_SHOW);
                SetForegroundWindow(hwnd); // ����ʾ����ʱ�������ڴ���ǰ̨
                return 0;
            }else if (LOWORD(wParam) == ID_EXIT) {
                DestroyWindow(hwnd);
                return 0;
            }
            break;
        // ���ƴ����е�����
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // // ��ȡ��ǰʱ��
            TextOutW(hdc, 0, 0, L"api-key:", lstrlenW(L"api-key:"));
            EndPaint(hwnd, &ps);
        } break;
        case WM_CREATE: {
            CreateChildWindow(hwnd,hwndChild, lParam);
        } break;
        case WM_SIZE:
             GetClientRect(hwnd, &rect) ;
             MoveWindow( hwndChild[ID_EDITBOX], 51, 0, rect.right, rect.bottom-30, TRUE ) ;        //�����ı��༭��
             MoveWindow( hwndChild[ID_SAVEBTN], rect.right - 110, rect.bottom-28, 50,  25, TRUE ) ;            //�������水ť
             MoveWindow( hwndChild[ID_CLSBTN ], rect.right - 55, rect.bottom-28, 50, 25, TRUE ) ;            //������հ�ť
             return 0 ;
        // ����һ����ʱ��
        // ��ʱ�����̣�NULL��ʾʹ�ô��ڹ��̣�
        // case WM_TIMER:
        //     switch (wParam) {
        //         case IDT_TIMER1:
        //             InvalidateRect(hwnd, NULL, TRUE); // ʹ����������Ч�������ػ�
        //             break;
        //     }
        //     break;
        case WM_CLOSE:
            ShowWindow(hwnd, SW_HIDE);
            return 0;

        case ID_SAVEBTN:{
            int iLength = GetWindowTextLength(hwndChild[ID_EDITBOX]) ;
            if( iLength != 0)
                szBuffer =(TCHAR*) malloc(iLength*2) ;
            else
                return -1 ;
            // GetWindowText( hwndChild[ID_EDITBOX], szBuffer, GetWindowTextLength(hwndChild[ID_EDITBOX]) + 1 ) ;
           
            if(GetWindowText( hwndChild[ID_EDITBOX], szBuffer, 256 ) < 1)
            {
                MessageBox(NULL, TEXT("api-key����Ϊ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION) ;
                return -1 ;
            }
            SavaInputContent( szBuffer, szBuffer ) ;
            return 0;
        }
             
        
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}



HWND Gui::hwndInput_ = nullptr;