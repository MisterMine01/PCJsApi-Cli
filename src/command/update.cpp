#ifndef PCJSAPI_COMMAND_UPDATE
#define PCJSAPI_COMMAND_UPDATE

#include "simp_cli/callback.hpp"

#include <string>
#include <iostream>
#include "./../cache/cache.hpp"
#include "./../utils.cpp"
#include "nlohmann/json.hpp"

using namespace mm1::simp_cli;

namespace pcjsapi
{
    class UpdateCallback : public Callback
    {
    private:
        std::string name;
        std::string description;
        pcjsapi::cache::Cache *cache = nullptr;

    public:
        UpdateCallback(pcjsapi::cache::Cache *cache)
        {
            this->cache = cache;
            name = "update";
            description = "update the project";
        }
        std::string get_name()
        {
            return name;
        }
        void print_help()
        {
            std::cout << description;
        }
        void run(std::vector<std::string> args)
        {
            (void)args;
            std::cout << "WARNING ! you can delete change in the index.php and .gitignore" << std::endl
                      << "update in this folder, are you ok ?" << std::endl
                      << "y/n[n]> ";
            std::string answer;
            std::cin >> answer;
            if (answer != "y")
            {
                std::cout << "cancel update" << std::endl;
                return;
            }
            pcjsapi::cache::CacheRelease release = this->cache->get_last_release();
            pcjsapi::cache::CacheAsset gitignore;
            pcjsapi::cache::CacheAsset zip_asset;
            getAsset(release, &gitignore, &zip_asset);
            if (gitignore.name == "" || zip_asset.name == "")
            {
                std::cerr << "Can't find gitignore or dev.zip in the last release" << std::endl;
                return;
            }
            nlohmann::json json;
            std::ifstream file("pcjs_config.json");
            file >> json;
            file.close();
            if (json["pcjs_version"] == release.tag_name)
            {
                std::cout << "You are already up to date" << std::endl;
                return;
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
            return;
        }
    };
}

#endif