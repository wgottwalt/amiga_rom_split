// SPDX-License-Identifier: LGPL-3.0-or-later
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Args.hxx"

constexpr int32_t Rom256k = 256 * 1024;
constexpr int32_t Rom512k = 512 * 1024;
constexpr int32_t Rom1024k = 1024 * 1024;
constexpr auto Bin = std::ios::binary;

void printUsage(const char *app)
{
    std::cout << "AMIGA ROM split and endianness correction - create burnable ROMs\n"
              << "usage: " << app << " <options>\n"
              << "\n"
              << "options:\n"
              << "  -h, --help        this help screen\n"
              << "  -f, --file <f>    input file\n"
              << "  -s, --split       also create splitted files for 32Bit Amigas\n"
              << "  -x, --suffix <s>  use suffix (default: .bin)\n"
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    Args args(argc, argv, nullptr, printUsage);

    if (!args.help(true))
    {
        auto [fname_avail, fname] = args.opt('f', "file", true);
        auto [split_avail, _] = args.opt('s', "split");
        auto [suffix_avail, suffix] = args.opt('x', "suffix", true);
        std::string fname_l;
        std::string fname_h;

        if (!fname_avail)
        {
            std::cerr << "error: no input file given" << std::endl;
            return 1;
        }

        if (auto pos = fname.find_last_of('.'); pos != std::string::npos)
        {
            fname_l = fname.substr(0, pos);
            fname_h = fname.substr(0, pos);
        }
        else
        {
            fname_l = fname;
            fname_h = fname;
        }

        if (split_avail)
        {
            fname_l += "_low";
            fname_h += "_high";
        }

        if (suffix_avail)
        {
            if (suffix[0] == '.')
                suffix.erase(0, 1);

            fname_l += "." + suffix;
            fname_h += "." + suffix;
        }
        else
        {
            fname_l += ".bin";
            fname_h += ".bin";
        }

        if (std::ifstream ifile(fname, Bin); ifile.is_open() && ifile.good())
        {
            const auto size = ifile.seekg(0, std::ios::end).tellg();
            std::vector<char> buf;

            if ((size != Rom256k) && (size != Rom512k) && (size != Rom1024k))
            {
                std::cerr << "error: wrong ROM size (needs to be 256, 512 or 512 KiB)" << std::endl;
                return 1;
            }

            buf.resize(size, 0);
            ifile.seekg(0, std::ios::beg);
            ifile.read(buf.data(), size);

            if (split_avail)
            {
                if (std::ofstream ofile_l(fname_l, Bin), ofile_h(fname_h, Bin);
                    ofile_l.is_open() && ofile_l.good() && ofile_h.is_open() && ofile_h.good())
                {
                    for (int32_t r = 0; r < 2; ++r)
                    {
                        for (int32_t i = 0; i < size; i += 4)
                        {
                            ofile_h.put(buf[i + 1]).put(buf[i]);
                            ofile_l.put(buf[i + 3]).put(buf[i + 2]);
                        }
                    }

                    std::cout << "created '" << fname_l << "' and '" << fname_h << "'" << std::endl;
                }
                else
                {
                    std::cerr << "error: unable to open output files '" << fname_l << "' and/or '"
                              << fname_h << "'" << std::endl;
                    return 1;
                }
            }
            else
            {
                if (std::ofstream ofile_l(fname_l, Bin); ofile_l.is_open() && ofile_l.good())
                {
                    for (int32_t i = 0; i < size; i += 4)
                        ofile_l.put(buf[i + 1]).put(buf[i]).put(buf[i + 3]).put(buf[i + 2]);

                    std::cout << "created '" << fname_l << "'" << std::endl;
                }
                else
                {
                    std::cerr << "error: unable to open output file '" << fname_l << "'"
                              << std::endl;
                    return 1;
                }
            }
        }
        else
        {
            std::cerr << "error: unable to open ROM file '" << fname << "'" << std::endl;
            return 1;
        }
    }

    return 0;
}
