#ifndef PCJSAPI_COMMAND_INIT
#define PCJSAPI_COMMAND_INIT

#include "simp_cli/callback.hpp"

#include <string>
#include <iostream>
#include "./../cache/cache.hpp"
#include "./../utils.cpp"
#include "nlohmann/json.hpp"

using namespace mm1::simp_cli;
using namespace pcjsapi;

namespace pcjsapi
{
    class InitCallback : public Callback
    {
    private:
        std::string name;
        std::string description;
        pcjsapi::cache::Cache *cache = nullptr;

    public:
        InitCallback(pcjsapi::cache::Cache *cache)
        {
            this->cache = cache;
            name = "init";
            description = "Initialize a new project";
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
            std::cout << "init in this folder are you ok ?" << std::endl
                      << "y/n[n]> ";
            std::string answer;
            std::cin >> answer;
            if (answer != "y")
            {
                std::cout << "cancel initialization" << std::endl;
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
            return;
        }
    };
}

#endif