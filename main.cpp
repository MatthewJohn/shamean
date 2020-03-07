#include <iostream>
#include <fstream>
#include <filesystem>

#include <CkCrypt2.h>

// 200KB default size to checksum
#define BYTE_LENGTH 204800

void ChecksumFile(char *filename)
{
    // This example assumes the Chilkat API to have been previously unlocked.
    // See Global Unlock Sample for sample code.

    CkCrypt2 crypt;

    // Set the name of the hash algorithm.
    // Other choices include "sha1", "sha256", "sha384", "sha512", "md2", "md5", and "haval". 
    crypt.put_HashAlgorithm("sha1");

    // EncodingMode specifies the encoding of the hash output.
    // It may be "hex", "url", "base64", or "quoted-printable".
    crypt.put_EncodingMode("hex");

    //
    int filesize_bytes = fs::file_size(filename);
    return;
    ifstream file(filename, ios::binary);

}
int main()
{
    cout << "Hello, World!" << endl;
    return 0;
}

