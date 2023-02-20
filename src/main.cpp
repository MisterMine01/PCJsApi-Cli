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
    return 0;
}
