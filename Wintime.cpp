// Wintime.cpp: 定义应用程序的入口点。
//

#include "Wintime.h"

using namespace std;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,      // handle to current instance
    _In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
    _In_ LPSTR lpCmdLine,          // command line
    _In_ int nCmdShow              // show state
){

    //SetConsoleCP(CP_UTF8);
    //// 设置控制台输出为UTF-8
    //SetConsoleOutputCP(CP_UTF8);
    //// SetConsoleOutputCP(936);
    //_setmode(_fileno(stdout), _O_U16TEXT);

    // 创建控制台

    Gui window(hInstance, "className", "图标名称");


    if (!window.RegisteClass()) {
        return -1;
    }

    if (!window.Create("xx程序")) {
        return -1;
    }

    if (!window.AddIcon()) {
        return -1;
    }



    window.Show(nCmdShow);
    watchwindows();
    //std::thread t(threadFunction);

    //t.detach();

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}