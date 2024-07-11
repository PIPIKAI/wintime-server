#include "gui.h"
#include "hook.h"
#include <mingw.thread.h>

// ����һ��Ҫ�����߳������еĺ���
void threadFunction() {
    watchwindows();
}

// ��WinMain������ʹ��SimpleWindow��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetConsoleCP(CP_UTF8);
    // ���ÿ���̨���ΪUTF-8
    SetConsoleOutputCP(CP_UTF8);
    // SetConsoleOutputCP(936);
    _setmode(_fileno(stdout), _O_U16TEXT);

    
    
    Gui window(hInstance ,"className" ,"ͼ������");

    
    if (!window.RegisteClass()) {
        return -1;
    }
    
    if (!window.Create("��س���")) {
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