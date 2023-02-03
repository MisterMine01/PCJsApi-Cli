#ifndef PCJS_GIT_HPP
#define PCJS_GIT_HPP

#include <string>

namespace pcjs {
    namespace git {
        struct Asset{
            std::string name;
            std::string url;
            std::string release_name;
        };

        struct Release{
            std::string name;
            std::string tag_name;
            std::string url;
            Asset* assets;
            int assets_size;
        };

        class GitData{
            private:
                std::string username;
                std::string repository;
                Release* releases;
                int releases_size;

            public:
                GitData(std::string username, std::string repository);
                //~GitData();
                Release* get_release(int* size);
                void download_asset(Asset asset, std::string path);
        };
    }
}

#endif