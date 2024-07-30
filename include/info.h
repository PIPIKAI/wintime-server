#pragma once
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
#include "api.hpp"


class History {

public:
    History();
    ~History();
    FileManager* fileManager = NULL;
    int max_size = 20;
    int upload();
    int clear();
    void preload();
    int push(Information);
private:
    std::vector<Information> Q;
};


static History history;