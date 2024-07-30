#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Information {
    int uid;
    char* process_name;
    char* windows_title;
    long long created_time;
    long long duration;
};

class Api {
public:
    int uid;
    Api(const std::string& base_api) : base_api(base_api) {}

    std::string auth(const std::string& token) {
        std::string url = base_api + "/auth";
        std::string post_fields = "token=" + token;

        return performRequest(url, post_fields);
    }

    std::string upload(const std::vector<Information>& info_vec) {
        std::string url = base_api + "/upload";
        json  json_data;

        for (const auto& info : info_vec) {
            json  item;
            item["uid"] = info.uid;
            item["process_name"] = info.process_name;
            item["windows_title"] = info.windows_title;
            item["created_time"] = info.created_time;
            item["duration"] = info.duration;
            json_data.push_back(item);
        }

        std::string post_fields = json_data.dump();

        return performRequest(url, post_fields);
    }

private:
    std::string base_api;

    std::string performRequest(const std::string& url, const std::string& post_fields) {
        CURL* curl;
        CURLcode res;
        std::string response;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return response;
    }

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};


static Api api;