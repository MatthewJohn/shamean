/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include <iostream>
#include <fstream>
#include <cstring>

#include "shamean.hpp"


int main( int argc, char* argv[] )
{
    s_options options;
    if (get_options(argc, argv, &options) || options.show_usage)
    {
        get_usage();
        return 1;
    }

    // Variable to hold output checksum.
    unsigned char checksum_bin[CHECKSUM_LENGTH];
    char checksum_hex[CHECKSUM_LENGTH_HEX];

    // Variable to hold if file error occurred
    bool file_err = false;

    // Perform checksum
    checksum_file(&options, checksum_bin, file_err);

    // Check if error occurred
    if (file_err)
    {
        std::cout << "shamean: cannot access '" << argv[1]
                  << "': No such file or directory" << std::endl;
        return 1;
    }

    convert_to_hex(checksum_bin, checksum_hex);

    std::cout << checksum_hex << std::endl;
    return 0;
}

