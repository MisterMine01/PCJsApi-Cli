#include <iostream>
#include "cache/cache.hpp"
#include "simp_cli/simp_cli.hpp"


#include "command/init.cpp"
#include "command/update.cpp"
#include "command/install.cpp"
#include "command/cli.cpp"

int main(int arc, char **argv)
{
    pcjsapi::cache::Cache cache("MisterMine01", "PCJsApi");
    mm1::simp_cli::SimpCli cli("pcjsapi", "1.0.0", "A cli for pcjsapi");
    cli.add_callback(new pcjsapi::InitCallback(&cache));
    cli.add_callback(new pcjsapi::UpdateCallback(&cache));
    cli.add_callback(new pcjsapi::InstallCallback(&cache));
    cli.add_callback(new pcjsapi::CLICallback(&cache));
    cli.run(arc, argv);

    /*
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
    } else if (args[1] == "update") {
        return pcjsapi::update(cache);
    } else if (args[1] == "install") {
        return pcjsapi::install(cache);
    } else if (args[1] == "build") {
        // TODO
    } else if (args[1] == "run") {
        // TODO
    } else if (args[1] == "add") {
        // TODO
    } else if (args[1] == "cli") {
        if (arc == 2) {
            return pcjsapi::cli_help();
        }
        std::string new_args[arc - 2];
        for (int i = 0; i < arc - 2; i++)
        {
            new_args[0] = args[i + 2];
        }
        return pcjsapi::cli_command(arc - 2, new_args, cache);
    }*/
    return 0;
}
