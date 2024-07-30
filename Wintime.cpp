﻿// Wintime.cpp: 定义应用程序的入口点。
//

#include "Wintime.h"

using namespace std;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,      // handle to current instance
    _In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
    _In_ LPSTR lpCmdLine,          // command line
    _In_ int nCmdShow              // show state
){

    wintime = Wintime(hInstance);

    MSG msg = wintime.Run();

    return static_cast<int>(msg.wParam);
}

Wintime::Wintime()
{
}

Wintime::Wintime(HINSTANCE hInstance)
{
    gui = Gui(hInstance, APP_NAME, APP_NAME);
    if (!gui.RegisteClass()) {
        exit(-1);
    }

    if (!gui.Create(APP_NAME)) {
        exit(-1);
    }

    if (!gui.AddIcon()) {
        exit(-1);
    }
    gui.Show(0);
    gui.setConsoleState();
    auto now = std::chrono::system_clock::now();
    last_timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    history = History();
}

MSG Wintime::Run()
{
    return watchwindows();
}

Wintime::~Wintime()
{
}
