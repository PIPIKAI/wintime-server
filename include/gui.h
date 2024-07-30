#ifndef __HEAD_GUI
#define __HEAD_GUI

#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <cwchar>
#include <io.h>
#include <fcntl.h>


#define ID_EXIT 100
#define SETTINGKEY 101
#define CONSOLEKEY 102
#define IDT_TIMER1 1001
#define FREASHTIME 500
#define ID_LIST_VIEW 500
#define WINDOW_WIDTH 300
#define WINDOW_HIGHT 100

class Gui
{
public:
    Gui(HINSTANCE hInstance, LPCSTR className, LPCSTR iconName);
    Gui();
    ~Gui();
    bool RegisteClass();
    bool Create(LPCSTR  appName);
    void Show(int nCmdShow);
    bool AddIcon();
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void ShowContextMenu(HWND hwnd_);
    static void setConsoleState();
private:
    HINSTANCE hInstance_;
    HWND hwnd_;
    static HWND hwndInput_;
    LPCSTR className_;
    LPCSTR iconName_;
    static int consleState ;

};

#endif // !__HEAD_GUI

