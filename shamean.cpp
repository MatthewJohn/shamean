/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include "shamean.hpp"

void checksum_file(const char *filename, unsigned char *checksum, bool &file_err)
{
    // Create struct to hold file info
    s_file_data file_data;

    // Leave room for start of file data, end of file data and file size.
    // @TODO Should it include timestamp?
    int buffer_size = sizeof(long) + (BYTE_LENGTH * 2);

    // Initialise array
    for (int i = 0; i < buffer_size; i++)
       file_data.all_data[i] = 0x00;

    // Open file to read data
    std::ifstream in_file(filename, std::ifstream::ate | std::ifstream::binary);

    if (! in_file.is_open())
    {
      file_err = true;
      return;
    }

    // Stop eating new lines in binary mode!!!
    in_file.unsetf(std::ios::skipws);

    // Seek to end of file, obtain file length and move back to start.
    in_file.seekg(0, std::ios::end);
    file_data.filesize = in_file.tellg();
    in_file.seekg(0, std::ios::beg);

    // Determine if file is big enough to read entire BYTE_LENGTH amount,
    // else read all available data
    int first_read_l = (file_data.filesize < BYTE_LENGTH) ? file_data.filesize : BYTE_LENGTH;

    // If there is data in file to read, do it.
    if (first_read_l > 0)
    {
        in_file.read(file_data.first, first_read_l);
    }

    // Determine amount of data to read from end of file.
    // If remainig data after read from first half, use either whole BYTE_LENGTH
    // or remaining data length.
    int end_read_l = ((file_data.filesize - first_read_l) < BYTE_LENGTH) ? (file_data.filesize - first_read_l) : BYTE_LENGTH;

    if (end_read_l > 0)
    {
        // Seek to file end minus size of read
        in_file.seekg(file_data.filesize - end_read_l);

        // Read end of file
        in_file.read(file_data.last, end_read_l);
    }


    // Create SHA object and initialise
    SHA_CTX ctx;
    SHA1_Init(&ctx);

    // Update SHA with all data from file_data struct
    SHA1_Update(&ctx, file_data.all_data, buffer_size);

    // Finalise checksum into checksum byte array
    SHA1_Final(checksum, &ctx);
    return;
}

void get_usage()
{
    std::cout << "Usage: shamain <Filename>" << std::endl
              << std::endl
              << "Generate a SHA-1 SUM, based on length and subset of data from a file."
              << std::endl
              << "Version: " << SHAMEAN_VERSION
              << ", GitCommit: " << SHAMEAN_GIT_COMMIT
              << ", BuildDate: " << SHAMEAN_BUILD_DATE << std::endl;
}

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

