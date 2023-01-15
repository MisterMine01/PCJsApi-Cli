#include <iostream>
#include "git/git.hpp"

int main(int arc, char **argv)
{
    pcjs::git::GitData git("MisterMine01", "PCJsApi");
    int size;
    pcjs::git::Release* releases = git.get_release(&size);
    for(int i = 0; i < size; i++){
        std::cout << "Release " << i << std::endl;
        std::cout << "    " << releases[i].name << std::endl;
        std::cout << "    " << releases[i].tag_name << std::endl;
        std::cout << "    " << releases[i].url << std::endl;
        for(int j = 0; j < releases[i].assets_size; j++){
            std::cout << "    " << "    " << releases[i].assets[j].name << std::endl;
            std::cout << "    " << "    " << releases[i].assets[j].url << std::endl;
            git.download_asset(releases[i].assets[j], "./" + releases[i].assets[j].name);
        }
    }
    return 0;
}
