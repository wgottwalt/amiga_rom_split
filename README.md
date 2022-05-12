AmigaRomSplit is a little tool for splitting a 512KiB 32bit ROM file (for
emulators or directly read out of the Amiga address space) into proper burnable
ROMs. This tool swaps endianness, splits lower and higher word of the dword
structure and also doubles the halfes to fill two 512KiB ROMs, just as required
by an Amiga with 32Bit bus structure (A1200, A3000, A4000).
