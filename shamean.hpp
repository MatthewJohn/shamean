/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <openssl/sha.h>

#include "version.hpp"

// 256KiB max data to checksum
#define MAX_BYTE_LENGTH 2 << 21
// 256KiB default size to checksum
#define DEFAULT_BYTE_LENGTH 2 << 18
#define CHECKSUM_LENGTH 20
#define CHECKSUM_LENGTH_HEX 40
#define PATH_MAX 4096

typedef struct {
    long filesize; ///< Size of file in bytes

    long last_modified; ///< Last modified date

    char first[MAX_BYTE_LENGTH]; ///< Data from start of file

    char last[MAX_BYTE_LENGTH]; ///< Data from end of file

} SFileData;

/**
 * Structure to hold command line arguments
 */
typedef struct {

    bool include_timestamp = false; ///< Whether to use timestamps in hash

    bool show_usage = false; ///< Whether to show usage and quit

    unsigned int byte_length = DEFAULT_BYTE_LENGTH; ///< Amount of bytes to use from start/end of tile

    char filename[PATH_MAX]; ///< Filename/path

} s_options;

/**
 * Read file and perform checksum.
 *
 * @param filename Input filename.
 * @param checksum Output buffer for checksum data.
 * @param file_err Set if an error occurred whilst reading file.
 */
void checksum_file(const s_options *options, unsigned char *checksum, bool &file_err);

/*
 * Obtain checksum of file and add to filedata
 *
 * @param options Options object
 * @param file_data Pointer to file data object
 *
 * @returns true if error occured during stat of file
 */
bool get_timestamp(const s_options *options, SFileData *file_data);

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
