// SPDX-License-Identifier: LGPL-3.0-or-later
#include <array>
#include <fstream>
#include <iostream>
#include <string>

constexpr int32_t RomSize = 512 * 1024;
constexpr auto Bin = std::ios::binary;

void printUsage(const char *app)
{
    std::cout << "AMIGA ROM split and endianness correction - create burnable ROMs\n"
              << "usage: " << app << " <512KiB rom file> [<option>]\n"
              << "\n"
              << "option:\n"
              << "  -h, --help   this help screen\n"
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    if (argc == 2)
    {
        const std::string fname = argv[1];

        if (fname == "-h" || fname == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }

        if (std::ifstream ifile(fname, Bin); ifile.is_open() && ifile.good())
        {
            const auto size = ifile.seekg(0, std::ios::end).tellg();
            std::array<char,RomSize> buf;

            if (size != RomSize)
            {
                std::cerr << "error: rom not suitable for splitting (needs to be 512KiB)"
                          << std::endl;
                return 1;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(buf.data(), RomSize);

            if (std::ofstream ofile_l(fname + "_low", Bin), ofile_h(fname + "_high", Bin);
                ofile_l.is_open() && ofile_l.good() && ofile_h.is_open() && ofile_h.good())
            {
                for (int32_t r = 0; r < 2; ++r)
                {
                    for (int32_t i = 0; i < RomSize; i += 4)
                    {
                        ofile_h.put(buf[i + 1]).put(buf[i]);
                        ofile_l.put(buf[i + 3]).put(buf[i + 2]);
                    }
                }

                std::cout << "created '" << (fname + "_low") << "' and '" << (fname + "_high")
                          << "'" << std::endl;
            }
            else
            {
                std::cerr << "error: unable to open output files '" << (fname + "_low")
                          << "' and/or '" << (fname + "_high") << "'" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << "error: unable to open rom '" << argv[1] << "'" << std::endl;
            return 1;
        }
    }
    else
        printUsage(argv[0]);

    return 0;
}
