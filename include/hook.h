#ifndef __HEAD_HOOK
#define __HEAD_HOOK

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Windows Vista及更高版本
#endif 

#include <fcntl.h>
#include <io.h>
#include <windows.h>

#include <chrono>
#include <cwchar>
#include <iomanip>
#include <iostream>

void watchwindows();

#endif // !__HEAD_HOOK