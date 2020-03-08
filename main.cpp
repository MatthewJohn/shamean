#include <iostream>
#include <fstream>
#include <cstring>

#include "shamean.hpp"

int main( int argc, const char* argv[] )
{
    if (argc != 2 or strcmp(argv[1], "--help") == 0) {
        get_usage();
        return 1;
    }

    // Variable to hold output checksum.
    unsigned char checksum[CHECKSUM_LENGTH];

    // Variable to hold if file error occurred
    bool file_err = false;

    // Perform checksum
    checksum_file(argv[1], checksum, file_err);

    // Check if error occurred
    if (file_err)
    {
        std::cout << "shamean: cannot access '" << argv[1]
                  << "': No such file or directory" << std::endl;
        return 1;
    }

    // Convert binary checksum into hex
    char output[2];
    for(int j = 0; j < CHECKSUM_LENGTH; j++)
    {
        // Print each character
        sprintf(output, "%02X", checksum[j]);
        std::cout << output;
    }
    std::cout << std::endl;
    return 0;
}

