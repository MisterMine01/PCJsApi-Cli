#ifndef PCJSAPI_COMMAND_CPP
#define PCJSAPI_COMMAND_CPP

#include "cache/cache.hpp"


namespace pcjsapi {
    int cli_update(pcjsapi::cache::Cache cache) {
        cache.update_cache(true);
        return 0;
    }
    int cli_clear() {
        std::experimental::filesystem::remove_all(ABSOLUTE_CACHE_PATH);
        return 0;
    }

    int cli_help() {
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi cli - Option of the cli" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi cli help - help" << std::endl;
        std::cout << "pcjsapi cli update - update the cache and install new version" << std::endl;
        std::cout << "pcjsapi cli clear - clear the cache" << std::endl;
        return 0;
    }

    int cli_command(int argc, std::string* argv, pcjsapi::cache::Cache cache) {
        if (argc == 0) {
            return cli_help();
        }
        if (argv[0] == "update") {
            return cli_update(cache);
        }
        if (argv[0] == "clear") {
            return cli_clear();
        }
        return cli_help();
    }

    int help() {
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi - For creating an php api simply" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "pcjsapi help - help" << std::endl;
        std::cout << "pcjsapi cli - option of the cli" << std::endl;
        return 0;
    }
};

#endif