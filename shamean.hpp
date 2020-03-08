/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include <iostream>
#include <fstream>
#include <cstring>

#include <openssl/sha.h>

#include "version.hpp"

// 200KB default size to checksum
#define BYTE_LENGTH 204800
#define CHECKSUM_LENGTH 20
#define CHECKSUM_LENGTH_HEX 40

typedef union {
    struct {

        // Size of file in bytes
        long filesize;

        // Data from start of file
        char first[BYTE_LENGTH];

        // Data from end of file
        char last[BYTE_LENGTH];
    };

    // Union of all data in file
    unsigned char all_data[sizeof(long) + (BYTE_LENGTH * 2)];
} s_file_data;

void checksum_file(const char *filename, unsigned char *checksum, bool &file_err);
void convert_to_hex(unsigned char *checksum_bin, char *checksum_hex);
void get_usage();

