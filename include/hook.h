#ifndef __HEAD_HOOK
#define __HEAD_HOOK

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Windows Vista及更高版本
#endif 

#include <fcntl.h>
#include <io.h>
#include <windows.h>
#include <shlobj.h>
#include <chrono>
#include <cwchar>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>
#include "filemanager.hpp"
#include "info.h"
MSG watchwindows();

static const char* APP_NAME = "Wintime";
static const char* filters[] = { "explorer.exe" };
static long long last_timestamp = 0;
static Information last_info = {};
#endif // !__HEAD_HOOK
