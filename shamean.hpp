/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>

#include <openssl/sha.h>

#include "version.hpp"

// 200KB default size to checksum
#define BYTE_LENGTH 204800
#define CHECKSUM_LENGTH 20
#define CHECKSUM_LENGTH_HEX 40

typedef union {
    struct {

        long filesize; ///< Size of file in bytes

        char first[BYTE_LENGTH]; ///< Data from start of file

        char last[BYTE_LENGTH]; ///< Data from end of file
    };

    unsigned char all_data[sizeof(long) + (BYTE_LENGTH * 2)]; ///< Union of all data for checksum
} SFileData;

// Structure to hold command line arguments
typedef struct {

    bool show_usage = false;  ///< Whether to show usage and quit

    char filename[];  ///< Filename/path

} s_options;

/**
 * Read file and perform checksum.
 *
 * @param filename Input filename.
 * @param checksum Output buffer for checksum data.
 * @param file_err Set if an error occurred whilst reading file.
 */
void checksum_file(const char *filename, unsigned char *checksum, bool &file_err);

/**
 * Convert char array of binary data to hex.
 *
 * @param checksum_bin Char array of binary checksum data.
 * @param checksum_hex Output buffer char array for hex to put.
 */
void convert_to_hex(unsigned char *checksum_bin, char *checksum_hex);

/**
 * Display usage
 */
void get_usage();

/**
 * Obtain command line options and build options object
 */
bool get_options(int argc, char* argv[], s_options* options);
