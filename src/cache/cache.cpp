#include <experimental/filesystem>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "./../git/git.hpp"
#include "cache.hpp"


pcjsapi::cache::Cache::Cache(std::string github_user, std::string github_repository)
{
    this->github_user = github_user;
    this->github_repository = github_repository;
    this->cache_path = ABSOLUTE_CACHE_PATH + github_user + "/" + github_repository + "/";
    if (!std::experimental::filesystem::exists(cache_path))
    {
        std::cout << "Creating cache directory..." << std::endl;
        std::experimental::filesystem::create_directories(cache_path);
        update_cache();
    } else {
        std::ifstream file(cache_path + "/cache.json");
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        buffer.clear();
        nlohmann::json json = nlohmann::json::parse(content);
        releases_size = json.size();
        releases = new CacheRelease[releases_size];
        for (int i = 0; i < releases_size; i++)
        {
            releases[i].name = json[i]["name"];
            releases[i].tag_name = json[i]["tag_name"];
            releases[i].assets_size = json[i]["assets"].size();
            releases[i].assets = new CacheAsset[releases[i].assets_size];
            for (int j = 0; j < releases[i].assets_size; j++)
            {
                releases[i].assets[j].name = json[i]["assets"][j]["name"];
                releases[i].assets[j].path = json[i]["assets"][j]["path"];
            }
        }
    }

}

pcjsapi::cache::Cache::~Cache()
{
}

pcjsapi::cache::CacheRelease *pcjsapi::cache::Cache::get_release(int *size)
{
    *size = releases_size;
    return releases;
}

pcjsapi::cache::CacheRelease pcjsapi::cache::Cache::get_last_release()
{
    return releases[releases_size - 1];
}

pcjsapi::cache::CacheRelease *pcjsapi::cache::Cache::find_release(std::string tag_name)
{
    for (int i = 0; i < releases_size; i++)
    {
        if (releases[i].tag_name == tag_name)
        {
            return &releases[i];
        }
    }
    return nullptr;
}

void pcjsapi::cache::Cache::update_cache(bool force)
{
    std::cout << "Updating cache..." << std::endl;
    // PCJSAPI REPOSITORY
    pcjs::git::GitData git_data("MisterMine01", "PCJsApi");

    int releases_size;
    pcjs::git::Release *releases = git_data.get_release(&releases_size);
    this->releases_size = releases_size;
    this->releases = new CacheRelease[releases_size];
    for (int i = 0; i < releases_size; i++)
    {
        this->releases[i].name = releases[i].name;
        this->releases[i].tag_name = releases[i].tag_name;
        this->releases[i].assets_size = releases[i].assets_size;
        this->releases[i].assets = new CacheAsset[releases[i].assets_size];
        for (int j = 0; j < releases[i].assets_size; j++)
        {
            this->releases[i].assets[j].name = releases[i].assets[j].name;
            this->releases[i].assets[j].path = std::experimental::filesystem::absolute(cache_path + "/" + releases[i].tag_name + "+" + releases[i].assets[j].name);
            if (force || (!std::experimental::filesystem::exists(this->releases[i].assets[j].path) && i == releases_size - 1))
            {
                std::cout << "Downloading " << releases[i].assets[j].name << "..." << std::endl;
                git_data.download_asset(releases[i].assets[j], this->releases[i].assets[j].path);
            }
        }
    }
    std::cout << "Saving cache..." << std::endl;
    nlohmann::json json = {};
    for (int i = 0; i < releases_size; i++)
    {
        json[i] = {
            {"name", releases[i].name},
            {"tag_name", releases[i].tag_name},
            {"assets", {}}
        };
        for (int j = 0; j < releases[i].assets_size; j++)
        {
            json[i]["assets"][j] = {
                {"name", releases[i].assets[j].name},
                {"path", this->releases[i].assets[j].path}
            };
        }
    }
    std::string s = json.dump(4);
    std::ofstream f;
    f.open(cache_path + "/cache.json");
    f << s;
    f.close();
}