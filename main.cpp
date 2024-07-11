#include "gui.h"
#include "hook.h"
#include <mingw.thread.h>

// 定义一个要在新线程中运行的函数
void threadFunction() {
    watchwindows();
}

// 在WinMain函数中使用SimpleWindow类
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetConsoleCP(CP_UTF8);
    // 设置控制台输出为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // SetConsoleOutputCP(936);
    _setmode(_fileno(stdout), _O_U16TEXT);

    
    
    Gui window(hInstance ,"className" ,"图标名称");

    
    if (!window.RegisteClass()) {
        return -1;
    }
    
    if (!window.Create("监控程序")) {
        return -1;
    }
    
    if (!window.AddIcon()) {
        return -1;
    }



    window.Show(nCmdShow);
    
    std::thread t(threadFunction);

    t.detach();

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg); 
        DispatchMessage(&msg);
    }
    
    return static_cast<int>(msg.wParam);
}