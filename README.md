
# shamean

### Generate a SHA-1 SUM, based on length and subset of data from a file.

    ./shamean <File>
    aaaabbbbccccffff0000

## Build

    ./scripts/build_container.sh
    docker run -v `pwd`:/code shamean-build ./scripts/build.sh

## What is this

### Background/uses

Shamean was developed for use whilst facing a task of performing (a multitude iterations of) checksums on terrabytes-worth of files (as part of a file-organising application, Vacker - https://github.com/MatthewJohn/vacker).

The files in particular were standard every-day files that you'd expect to find on a PC, e.g. photos, videos, movies etc.

Shamean was written to checksum the bare-minimum of a file to get a rough idea of whether it is a duplicate of another, by checksumming a small portion of a given file.

Using this, for the ~99% (some high percentage picked from a hat) of files that gave different results (as the portion read was unique), the effort to read/checksum the rest of the file was saved.

From there, for the remaining small percentage of files that the checksum matches, a second checksum of the whole file (using sha512sum or such) can be performed.

Since most files store metadata at the start of the file, which differs between most files, the checksum from shamean becomes reasonably accurate (personally speaking).


### What it is NOT

Do not use shamean to determine if files are identical. Period.

Identical checksums do not mean identical file content.

Therefore, I strongly recommend not using this as a file comparison for duplicate-purging purposes or other like-tasks.

## Examples

```
## Files with the same content
$ echo "Identical file" > ./file_a
$ echo "Identical file" > ./file_b

$ ./shamean ./file_a
55DC8CF43FBE191C52A244A67C881167DFD998E5
$ ./shamean ./file_b
55DC8CF43FBE191C52A244A67C881167DFD998E5

## Files with the different content
$ echo "Identical file" > ./file_a
$ echo "Different file" > ./file_b

$ ./shamean ./file_a
55DC8CF43FBE191C52A244A67C881167DFD998E5
$ ./shamean ./file_b
3149A0A2FEA93E88DB30D39383FAFA1D267E3777

## Checking file modification timestamps
$ echo "File Content" > ./file_a
$ cp -a ./file_a ./file_b

$ ./shamean -t ./file_a
BD16D99AC58588C3B48D7558D7BC60E9FDE74071
$ ./shamean -t ./file_b
BD16D99AC58588C3B48D7558D7BC60E9FDE74071

$ touch ./file_b
$ ./shamean -t ./file_b
79EA66DE835D2BBF8599F127B2933690D5EFA2D5
```


## Uses in pop-culture

Cindy Campbell (Scary Movie): `Yo, that jacket is tight, son. shamean`

Sean Kingston (Replay): `Shamean like a melody in my head`

Pretty sure Gandalf said something about it


## Developmnet

NOTE: This repo is a replica of DS phabricator. Do not commit to any mirror.

Use the supplied Jenkinsfile to run CI.

Required packages:

    g++
    libssl-dev
    libcrypto++-dev
    libcppunit-dev


Build for tests:

    ./scripts/build_container.sh
    docker run -v `pwd`:/code shamean-build ./scripts/run_tests.sh


