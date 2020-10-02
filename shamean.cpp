/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include "shamean.hpp"

void checksum_file(const s_options *options, unsigned char *checksum, bool &file_err)
{
    // Create struct to hold file info
    SFileData file_data;

    // Initialise array
    file_data.filesize = 0;
    file_data.last_modified = 0;
    for (unsigned int i = 0; i < options->byte_length; i++)
       file_data.first[i] = 0x00;
    for (unsigned int i = 0; i < options->byte_length; i++)
       file_data.last[i] = 0x00;

    if (options->include_timestamp)
    {
        if (!get_timestamp(options, &file_data))
        {
            std::cout << "Error whilst getting file stat()" << std::endl;
        }
    }

    // Open file to read data
    std::ifstream in_file(options->filename, std::ifstream::ate | std::ifstream::binary);

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
    int first_read_l = (file_data.filesize < options->byte_length) ? file_data.filesize : options->byte_length;

    // If there is data in file to read, do it.
    if (first_read_l > 0)
    {
        in_file.read(file_data.first, first_read_l);
    }

    // Determine amount of data to read from end of file.
    // If remainig data after read from first half, use either whole BYTE_LENGTH
    // or remaining data length.
    int end_read_l = ((file_data.filesize - first_read_l) < options->byte_length) ? (file_data.filesize - first_read_l) : options->byte_length;

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
    SHA1_Update(&ctx, &file_data.filesize, sizeof(long));
    SHA1_Update(&ctx, &file_data.first, options->byte_length);
    SHA1_Update(&ctx, &file_data.last, options->byte_length);

    if (options->include_timestamp)
    {
        SHA1_Update(&ctx, &file_data.last_modified, sizeof(long));
    }

    // Finalise checksum into checksum byte array
    SHA1_Final(checksum, &ctx);
    return;
}

bool get_timestamp(const s_options *options, SFileData *file_data)
{
    struct stat result;
    if (stat(options->filename, &result) == 0)
    {
        file_data->last_modified = result.st_mtime;
        return true;
    }
    else
    {
        return false;
    }
}

void convert_to_hex(unsigned char *checksum_bin, char *checksum_hex)
{
    // Convert binary checksum into hex
    for(unsigned int j = 0; j < CHECKSUM_LENGTH; j++)
    {
        // Print each character
        sprintf(&checksum_hex[j * 2], "%02X", checksum_bin[j]);
    }
}

void get_usage()
{
    std::cout << "Usage: shamain [-t] [-h] <Filename>" << std::endl
              << std::endl
              << "Generate a SHA-1 SUM, based on length and subset of data from a file."
              << std::endl
              << "Version: " << SHAMEAN_VERSION
              << ", GitCommit: " << SHAMEAN_GIT_COMMIT
              << ", BuildDate: " << SHAMEAN_BUILD_DATE << std::endl;
}


bool get_options(int argc, char* argv[], s_options* options)
{
    int option;

    while ((option = getopt(argc, argv, "thb")) != -1) {
        switch(option) {
            case 'h':
                options->show_usage = true;
                break;
            case 't':
                options->include_timestamp = true;
                break;
            case 'b':
		options->byte_length = atoi(optarg);
		break;
            case '?': //used for some unknown options
                printf("unknown option: %c\n", optopt);
                return true;
        }
    }

    // Return error if there is not one remaining argument (file path)
    if (argc - optind != 1)
    {
        return true;
    }

    if (options->byte_length < 1)
    {
        std::cout << "Byte length must a positive integer" << std::endl;
    }

    if (options->byte_length > MAX_BYTE_LENGTH)
    {
        std::cout << "Byte length must be a maximum of " << MAX_BYTE_LENGTH << std::endl;
        return true;
    }

    // Move filename into filename option
    for(; optind < argc; optind++) {
        strcpy(options->filename, argv[optind]);
    }

    // Return everything's okay
    return false;
}
