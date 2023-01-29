#ifndef PCJSAPI_COMMAND_CPP
#define PCJSAPI_COMMAND_CPP

#include <string>
#include <iostream>
#include "cache/cache.hpp"
#include <zip.h>
#include <experimental/filesystem>
#include <fstream>
#include "nlohmann/json.hpp"

namespace pcjsapi
{
    int help()
    {
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi - For creating an php api simply" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi help - help" << std::endl;
        std::cout << "pcjsapi cli - option of the cli" << std::endl;
        return 0;
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
        for (int i = 0; i < release.assets_size; i++)
        {
            if (release.assets[i].name == "gitignore")
            {
                gitignore = release.assets[i];
            }
            else if (release.assets[i].name == "dev.zip")
            {
                zip_asset = release.assets[i];
            }
        }
        std::experimental::filesystem::copy_file(gitignore.path, ".gitignore");
        struct zip *z;
        struct zip_file *zfile;
        struct zip_stat zstat;
        int err;
        if ((z = zip_open(zip_asset.path.c_str(), 0, &err)) == NULL)
        {
            std::cerr << "Can't open zip_file: " << zip_asset.name << std::endl;
            return 1;
        }
        for (int i = 0; i < zip_get_num_entries(z, 0); i++)
        {
            if (!zip_stat_index(z, i, 0, &zstat) == 0)
            {
                printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
                continue;
            }
            if (zstat.name[strlen(zstat.name) - 1] == '/')
            {
                std::experimental::filesystem::create_directories(zstat.name);
                continue;
            }
            if ((zfile = zip_fopen_index(z, i, 0)) == NULL)
            {
                printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
                continue;
            }
            std::ofstream file(zstat.name);
            char buf[1024];
            int len;
            while ((len = zip_fread(zfile, buf, sizeof(buf))) > 0)
            {
                file.write(buf, len);
            }
            file.close();
            zip_fclose(zfile);
        }
        zip_close(z);
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
}

#endif