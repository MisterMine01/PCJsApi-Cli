#ifndef PCJSAPI_COMMAND_CLI_CLEAR
#define PCJSAPI_COMMAND_CLI_CLEAR

#include "simp_cli/callback.hpp"
#include "./../../cache/cache.hpp"
#include <string>
#include <iostream>

using namespace mm1::simp_cli;

namespace pcjsapi
{
    class CliClear : public Callback
    {
    private:
        std::string name;
        std::string description;
        pcjsapi::cache::Cache *cache = nullptr;

    public:
        CliClear(pcjsapi::cache::Cache *cache)
        {
            this->cache = cache;
            name = "clear";
            description = "clear the cache";
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
            std::experimental::filesystem::remove_all(ABSOLUTE_CACHE_PATH);
        }
    };
}

#endif