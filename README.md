AmigaRomSplit is a little tool for preparing Amiga Kickstart files to files
suitable for burning onto ROMs. This tool swaps endianness, splits lower and
higher word of the dword structure and also doubles the halfes to fill two ROMs,
just as required by an Amiga with 32Bit bus structure (A1200, A3000, A4000).

Its written having plain C++20 and Linux/BSD in mind, but can be easily changed
to only require C++17 and to compile on other OSes.
