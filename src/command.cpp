#ifndef PCJSAPI_COMMAND_CPP
#define PCJSAPI_COMMAND_CPP

#include <string>
#include <iostream>
#include "cache/cache.hpp"
#include <experimental/filesystem>
#include "nlohmann/json.hpp"
#include "zipper.cpp"

namespace pcjsapi
{
    int help()
    {
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi - For creating an php api simply" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi help - help" << std::endl;
        std::cout << "pcjsapi cli - option of the cli" << std::endl;
        std::cout << "pcjsapi init - initialize the project" << std::endl;
        std::cout << "pcjsapi update - update the project" << std::endl;
        std::cout << "pcjsapi install - install the dependency of the project" << std::endl;
        return 0;
    }

    void getAsset(pcjsapi::cache::CacheRelease release, pcjsapi::cache::CacheAsset *gitignore, pcjsapi::cache::CacheAsset *zip_asset)
    {
        for (int i = 0; i < release.assets_size; i++)
        {
            if (release.assets[i].name == "gitignore")
            {
                *gitignore = release.assets[i];
            }
            else if (release.assets[i].name == "dev.zip")
            {
                *zip_asset = release.assets[i];
            }
        }
    }

    int init(pcjsapi::cache::Cache cache)
    {
        std::cout << "init in this folder are you ok ?" << std::endl
                  << "y/n[n]> ";
        std::string answer;
        std::cin >> answer;
        if (answer != "y")
        {
            std::cout << "cancel initialization" << std::endl;
            return 1;
        }
        pcjsapi::cache::CacheRelease release = cache.get_last_release();
        pcjsapi::cache::CacheAsset gitignore;
        pcjsapi::cache::CacheAsset zip_asset;
        getAsset(release, &gitignore, &zip_asset);
        if (gitignore.name == "" || zip_asset.name == "")
        {
            std::cerr << "Can't find gitignore or dev.zip in the last release" << std::endl;
            return 1;
        }
        if (std::experimental::filesystem::exists(".gitignore"))
        {
            std::experimental::filesystem::remove(".gitignore");
        }
        std::experimental::filesystem::copy_file(gitignore.path, ".gitignore");

        decompress(zip_asset.path, true);
        
        std::experimental::filesystem::create_directories("src");
        nlohmann::json json = {
            {"pcjs_version", release.tag_name},
            {"components_dir", "src"},
            {"packages", {}},
            {"error_entries", {{"no_entry", "PcJsApi.__not_entry"}, {"not_found", "PcJsApi.__not_found"}, {"unknown_error", "PcJsApi.__unknown_error"}, {"not_public", "PcJsApi.__not_public"}, {"no_parameters", "PcJsApi.__not_parameters"}}}};
        std::ofstream file("pcjs_config.json");
        file << json.dump(4);
        file.close();
        return 0;
    }

    int update(pcjsapi::cache::Cache cache)
    {
        std::cout << "WARNING ! you can delete change in the index.php and .gitignore" << std::endl
                << "update in this folder, are you ok ?" << std::endl
                << "y/n[n]> ";
        std::string answer;
        std::cin >> answer;
        if (answer != "y")
        {
            std::cout << "cancel update" << std::endl;
            return 1;
        }
        pcjsapi::cache::CacheRelease release = cache.get_last_release();
        pcjsapi::cache::CacheAsset gitignore;
        pcjsapi::cache::CacheAsset zip_asset;
        getAsset(release, &gitignore, &zip_asset);
        if (gitignore.name == "" || zip_asset.name == "")
        {
            std::cerr << "Can't find gitignore or dev.zip in the last release" << std::endl;
            return 1;
        }
        nlohmann::json json;
        std::ifstream file("pcjs_config.json");
        file >> json;
        file.close();
        if (json["pcjs_version"] == release.tag_name)
        {
            std::cout << "You are already up to date" << std::endl;
            return 0;
        }
        if (std::experimental::filesystem::exists(".gitignore"))
        {
            std::experimental::filesystem::remove(".gitignore");
        }
        std::experimental::filesystem::copy_file(gitignore.path, ".gitignore");

        decompress(zip_asset.path, true);

        json["pcjs_version"] = release.tag_name;
        std::ofstream file2("pcjs_config.json");
        file2 << json.dump(4);
        file2.close();
        return 0;
    }
}

#endif