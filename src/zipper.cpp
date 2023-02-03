#ifndef PCJSAPI_ZIPPER_CPP
#define PCJSAPI_ZIPPER_CPP

#include <zip.h>
#include <string>
#include <iostream>
#include <string.h>
#include <experimental/filesystem>
#include <fstream>

namespace pcjsapi
{
    int decompress(std::string path_zip, bool force = false, bool autocancel = false)
    {
        std::cout << "Decompressing " << path_zip << std::endl;
        struct zip *z;
        struct zip_file *zfile;
        struct zip_stat zstat;
        int err;
        if ((z = zip_open(path_zip.c_str(), 0, &err)) == NULL)
        {
            std::cerr << "Can't open zip_file: " << path_zip << std::endl;
            return 1;
        }

        for (int i = 0; i < zip_get_num_entries(z, 0); i++)
        {
            if (!zip_stat_index(z, i, 0, &zstat) == 0)
            {
                printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
                continue;
            }
            if (zstat.name[strlen(zstat.name) - 1] == '/')
            {
                if (std::experimental::filesystem::exists(zstat.name))
                {
                    continue;
                }
                std::experimental::filesystem::create_directories(zstat.name);
                continue;
            }
            if ((zfile = zip_fopen_index(z, i, 0)) == NULL)
            {
                printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
                continue;
            }
            if (std::experimental::filesystem::exists(zstat.name))
            {
                if (!force)
                {
                    std::cout << "File " << zstat.name << " already exists, do you want to overwrite it ?" << std::endl
                              << "y/n[n]> ";
                    std::string answer;
                    std::cin >> answer;
                    if (answer != "y")
                    {
                        std::cout << "cancel overwrite" << std::endl;
                        continue;
                    }
                }
                if (autocancel)
                {
                    continue;
                }
            }
            std::ofstream file(zstat.name);
            char buf[1024];
            int len;
            while ((len = zip_fread(zfile, buf, sizeof(buf))) > 0)
            {
                file.write(buf, len);
            }
            file.close();
            zip_fclose(zfile);
        }
        zip_close(z);
        return 0;
    }

} // namespace pcjsapi

#endif