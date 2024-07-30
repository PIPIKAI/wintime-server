#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <mutex>
//#include <codecvt>
#include <shlobj.h> // For SHGetFolderPath
#include <stdexcept>

namespace fs = std::filesystem;



class FileManager {
public:
    FileManager() {

    }

    FileManager(const char* app_name, const char* filename) {
        // 获取用户的appdata目录路径
        char appdata[1024];

        if (! SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appdata))) {
            throw std::runtime_error("Error: Failed to get AppData folder path.");
        }

        // 创建app_name文件夹路径
        appDir = std::string(appdata) + "\\" + app_name;
        if (!fs::exists(appDir) && !fs::create_directories(appDir)) {
            throw std::runtime_error("Error creating directory: " + appDir);
        }

        // 设置文件路径
        filePath = appDir + "\\" + filename;

        // 如果文件不存在，则创建文件
        if (!fs::exists(filePath)) {
            std::ofstream file(filePath);
            if (!file.is_open()) {
                throw std::runtime_error("Error creating file: " + filePath);
            }
            file.close();
        }
    }

    bool insert(const char* s) {
        std::lock_guard<std::mutex> guard(fileMutex);

        std::ofstream file(filePath, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << filePath << std::endl;
            return false;
        }

        file << s << "\n";
        return true;
    }

    std::string get() {
        std::lock_guard<std::mutex> guard(fileMutex);
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error opening file for reading: " << filePath << std::endl;
            return "";
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    }

    bool clear() {
        std::lock_guard<std::mutex> guard(fileMutex);
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error opening file for clearing: " << filePath << std::endl;
            return false;
        }
        return true;
    }

private:
    std::string appDir;
    std::string filePath;
    std::mutex fileMutex;
};

