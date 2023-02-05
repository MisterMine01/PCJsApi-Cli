#ifndef PCJSAPI_COMMAND_CLI
#define PCJSAPI_COMMAND_CLI

#include "simp_cli/callback.hpp"

#include <string>
#include <iostream>
#include <vector>
#include "./../cache/cache.hpp"

using namespace mm1::simp_cli;

namespace pcjsapi {
    class CLICallback : public Callback {
        private:
            std::string name;
            std::string description;
            pcjsapi::cache::Cache* cache = nullptr;
            std::vector<Callback*> callbacks;
        public:
            CLICallback(pcjsapi::cache::Cache* cache) {
                this->cache = cache;
                name = "cli";
            }
            std::string get_name() {
                return name;
            }
            void print_help() {
                std::cout << name;

            }
            void run(std::vector<std::string> args) {
                std::cout << "CLI";
            }

    };
}

#endif
