#include "info.h"


// 解析单行字符串到 Information 结构体
static Information parseLine(const std::string& line) {
    Information info;
    std::stringstream ss(line);

    std::string uid_str, process_name, windows_title, created_time_str, duration_str;

    // 读取字段
    std::getline(ss, uid_str, ',');
    std::getline(ss, process_name, ',');
    std::getline(ss, windows_title, ',');
    std::getline(ss, created_time_str, ',');
    std::getline(ss, duration_str);

    // 解析字段到结构体
    info.uid = std::stoi(uid_str);
    info.process_name = strdup(process_name.c_str());
    info.windows_title = strdup(windows_title.c_str());
    info.created_time = std::stoll(created_time_str);
    info.duration = std::stoll(duration_str);

    return info;
}


History::History()
{
    fileManager = new FileManager("Wintime", "CACHE");
    preload();

}

History::~History()
{
}

int History::upload()
{
    return 1;
}

int History::clear()
{
    Q.clear();
    fileManager->clear();
    return 0;
}

void History::preload()
{
    std::string fileContent = fileManager->get();
    std::stringstream ss(fileContent);
    std::string line;
    // 逐行解析文件内容
    while (std::getline(ss, line)) {
        if (!line.empty()) {
            Information info = parseLine(line);
            Q.emplace_back(info);
        }
    }
}

int History::push(Information i)
{
    Q.emplace_back(i);
    char info[1024];
    sprintf(info, "%d,%s,%s,%lld,%lld", i.uid, i.process_name, i.windows_title, i.created_time, i.duration);
    fileManager->insert(info);

    if (Q.size() >= max_size) {
        if (upload()) {
            clear();
        }
    }
    return 0;
}
