#include <iostream>
#include "cache/cache.hpp"
#include "cli_command.cpp"
#include "command.cpp"

int main(int arc, char **argv)
{
    pcjsapi::cache::Cache cache;
    std::string *args = new std::string[arc];
    for (int i = 0; i < arc; i++)
    {
        args[i] = argv[i];
    }
    if (arc == 1  || args[1] == "help")
    {
        return pcjsapi::help();
    }
    if (args[1] == "init")
    {
        return pcjsapi::init(cache);
    }
    if (args[1] == "cli") {
        if (arc == 2) {
            return pcjsapi::cli_help();
        }
        std::string new_args[arc - 2];
        for (int i = 0; i < arc - 2; i++)
        {
            new_args[0] = args[i + 2];
        }
        return pcjsapi::cli_command(arc - 2, new_args, cache);
    }
    return 0;
}
