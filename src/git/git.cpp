#include "git.hpp"
#include <nlohmann/json.hpp>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


pcjs::git::GitData::GitData(std::string username, std::string repository)
{
    this->username = username;
    this->repository = repository;
    this->releases = NULL;
    this->releases_size = 0;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, ("https://api.github.com/repos/" + this->username + "/" + this->repository + "/releases").c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "PCJsApi-CLI/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    } else {
        throw "Curl error";
    }
    nlohmann::json json = nlohmann::json::parse(readBuffer);
    this->releases_size = json.size();
    this->releases = new pcjs::git::Release[this->releases_size];
    for(int i = 0; i < this->releases_size; i++){
        this->releases[i].name = json[i]["name"];
        this->releases[i].tag_name = json[i]["tag_name"];
        this->releases[i].url = json[i]["url"];
        this->releases[i].assets_size = json[i]["assets"].size();
        this->releases[i].assets = new pcjs::git::Asset[this->releases[i].assets_size];
        for(int j = 0; j < this->releases[i].assets_size; j++){
            this->releases[i].assets[j].name = json[i]["assets"][j]["name"];
            this->releases[i].assets[j].url = json[i]["assets"][j]["browser_download_url"];
            this->releases[i].assets[j].release_name = this->releases[i].name;
        }
    }
}

pcjs::git::Release* pcjs::git::GitData::get_release(int* size)
{
    *size = this->releases_size;
    return this->releases;
}

void pcjs::git::GitData::download_asset(pcjs::git::Asset asset, std::string path)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;

    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        fp = fopen(path.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, asset.url.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "PCJsApi-CLI/1.0");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}