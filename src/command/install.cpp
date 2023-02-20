#ifndef PCJSAPI_COMMAND_INSTALL
#define PCJSAPI_COMMAND_INSTALL

#include "simp_cli/callback.hpp"

#include <string>
#include <iostream>
#include "./../cache/cache.hpp"
#include "./../utils.cpp"
#include "nlohmann/json.hpp"

using namespace mm1::simp_cli;

namespace pcjsapi
{
    class InstallCallback : public Callback
    {
    private:
        std::string name;
        std::string description;
        pcjsapi::cache::Cache *cache = nullptr;

    public:
        InstallCallback(pcjsapi::cache::Cache *cache)
        {
            this->cache = cache;
            name = "install";
            description = "install the dependency of the project";
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
            std::cout << "install in this folder, are you ok ?" << std::endl
                      << "y/n[n]> ";
            std::string answer;
            std::cin >> answer;
            if (answer != "y")
            {
                std::cout << "cancel install" << std::endl;
                return;
            }
            nlohmann::json json;
            std::ifstream file("pcjs_config.json");
            file >> json;
            file.close();
            std::string version = json["pcjs_version"];
            pcjsapi::cache::CacheRelease *release = this->cache->find_release(version);
            if (release == nullptr)
            {
                std::cerr << "Can't find the release " << version << std::endl;
                return;
            }
            pcjsapi::cache::CacheAsset zip_asset;
            for (int i = 0; i < release->assets_size; i++)
            {
                if (release->assets[i].name == "dev.zip")
                {
                    zip_asset = release->assets[i];
                }
            }
            if (zip_asset.name == "")
            {
                std::cerr << "Can't find dev.zip in the release" << std::endl;
                return;
            }
            decompress(zip_asset.path, false, false);
            return;
        }
    };
}

#endif