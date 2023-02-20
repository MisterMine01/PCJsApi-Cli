#ifndef PCJSAPI_COMMAND_CLI_UPDATE
#define PCJSAPI_COMMAND_CLI_UPDATE

#include "simp_cli/callback.hpp"
#include "./../../cache/cache.hpp"
#include <string>
#include <iostream>


using namespace mm1::simp_cli;

namespace pcjsapi {
    class CliUpdate: public Callback {
        private:
            std::string name;
            std::string description;
            pcjsapi::cache::Cache* cache = nullptr;
        public:
            CliUpdate(pcjsapi::cache::Cache* cache) {
                this->cache = cache;
                name = "update";
                description = "update the dependency of the project";
            }
            std::string get_name() {
                return name;
            }
            void print_help() {
                std::cout << description;
            }
            void run(std::vector<std::string> args) {
                (void)args;
                this->cache->update_cache(true);
            }

    };
}

#endif