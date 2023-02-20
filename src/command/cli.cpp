#ifndef PCJSAPI_COMMAND_CLI
#define PCJSAPI_COMMAND_CLI

#include "simp_cli/callback.hpp"

#include <string>
#include <iostream>
#include <vector>
#include "./../cache/cache.hpp"
#include "./cli/clear.cpp"
#include "./cli/update.cpp"

using namespace mm1::simp_cli;

namespace pcjsapi
{
    class CLICallback : public Callback
    {
    private:
        std::string name;
        std::string description;
        pcjsapi::cache::Cache *cache = nullptr;
        std::vector<Callback *> callbacks;

    public:
        CLICallback(pcjsapi::cache::Cache *cache)
        {
            this->cache = cache;
            name = "cli";
            description = "settings of the cli";
            callbacks.push_back(new CliClear(cache));
            callbacks.push_back(new CliUpdate(cache));
        }
        std::string get_name()
        {
            return name;
        }
        void print_help()
        {
            std::cout << description << std::endl;
            for (auto callback : callbacks)
            {
                std::cout << "   " << callback->get_name() << ": ";
                callback->print_help();
                std::cout << std::endl;
            }
        }
        void run(std::vector<std::string> args)
        {
            if (args.size() == 0 || args.at(0) == "help")
            {
                // HELP
                for (auto callback : this->callbacks)
                {
                    std::cout << callback->get_name() << ": ";
                    callback->print_help();
                    std::cout << std::endl;
                }
                return;
            }
            std::string name = args.at(0);
            for (auto call : this->callbacks)
            {
                if (call->get_name() == name)
                {
                    std::vector<std::string> copy(args);
                    copy.erase(copy.begin());
                    call->run(copy);
                    return;
                }
            }
            // HELP
            for (auto callback : this->callbacks)
            {
                std::cout << callback->get_name() << ": ";
                callback->print_help();
                std::cout << std::endl;
            }
        }
    };
}

#endif
