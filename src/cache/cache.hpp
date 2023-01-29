#ifndef PCJSAPI_CACHE_HPP
#define PCJSAPI_CACHE_HPP

#define CACHE_PATH "pcjsapi/"
#define ABSOLUTE_CACHE_PATH getenv("HOME") + std::string("/.cache/") + CACHE_PATH

#include <string>
#include <stdlib.h>


namespace pcjsapi
{
    namespace cache
    {
        struct CacheAsset
        {
            std::string name;
            std::string path;
        };

        struct CacheRelease
        {
            std::string name;
            std::string tag_name;
            CacheAsset *assets;
            int assets_size;
        };

        class Cache
        {
        private:
            CacheRelease *releases;
            int releases_size;

        public:
            Cache();
            ~Cache();
            CacheRelease *get_release(int *size);
            CacheRelease get_last_release();
            CacheRelease *find_release(std::string tag_name);
            void update_cache(bool force = false);
            void download_asset(CacheAsset asset);
        };
    };
};

#include "./cache.cpp"

#endif