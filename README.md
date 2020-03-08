
# shamean

### Generate a SHA-1 SUM, based on length and subset of data from a file.

    ./shamean <File>
    aaaabbbbccccffff0000

## Build

    docker build . -t shamean-build -f Dockerfile.build
    docker run -v `pwd`:/code shamean-build bash -c "cd /code; g++ -g main.cpp -o shamean -lcrypto -static"

## Uses in pop-culture

Cindy Campbell (Scary Movie): `Yo, that jacket is tight, son. shamean`
Sean Kingston (Replay): `Shamean like a melody in my head`

Pretty sure Gandalf said something about it

