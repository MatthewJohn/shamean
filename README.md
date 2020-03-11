
# shamean

### Generate a SHA-1 SUM, based on length and subset of data from a file.

    ./shamean <File>
    aaaabbbbccccffff0000

## Build

    docker build . -t shamean-build -f Dockerfile.build
    docker run -v `pwd`:/code shamean-build bash -c "cd /code; g++ -g main.cpp -o shamean -lcrypto -static"

## What is this

### Background/uses

Shamean was developed for use whilst facing a task of performing (a multitude iterations of) checksums on terrabytes-worth of files (as part of a file-organising application, Vacker - https://github.com/MatthewJohn/vacker).

The files in particular were standard every-day files that you'd expect to find on a PC, e.g. photos, videos, movies etc.

Shamean was written to checksum the bare-minimum of a file to get a rough idea of whether it is a duplicate of another, by checksumming a small portion of a given file.

Using this, for the ~99% (some high percentage picked from a hat) of files that gave different results (as the portion read was unique), the effort to read/checksum the rest of the file was saved.

From there, for the remaining small percentage of files that the checksum matches, a second checksum of the whole file (using sha512sum or such) can be performed.

Since most files store metadata at the start of the file, which differs between most files, the checksum from shamean becomes reasonably accurate (personally speaking).


### What it is NOT

Do not use shamean to determine if files are identicle. Period.

Identicle checksums do not mean identical file content.

Therefore, I strongly recommend not using this as a file comparison for duplicate-purging purposes or other like-tasks.


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
    docker run -v `pwd`:/code shamean-build bash -c 'cd /code; ./scripts/run_tests.sh'


