#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>


int err() {
    printf("Error %d: %s\n", errno, strerror(errno));
    exit(1);
}



//takes 'byte' and zeros out all least significant bits up to and including 'num' and inserts 'insert' into those bits.
unsigned char leastSigBit(unsigned char byte, int num, int insert){
  return byte - (byte % ((int) pow(2,num))) + insert;
}



//assumes fd is at start of file, returns 0 if not WAV and data size if WAV
int checkWav(int fd) {
    char* s = malloc(5);
    if(read(fd, s, 4) < 4) return 0;
    if(strcmp(s, "RIFF") != 0) return 0;
    if(read(fd, s, 4) < 4) return 0;
    //file size - 8 is stored in s now
    if(read(fd, s, 4) < 4) return 0;
    if(strcmp(s, "WAVE") != 0) return 0;
    if(read(fd, s, 4) < 4) return 0; // next subchunk header is now in data
    int lastHeader = 12;
    while(strcmp(s, "data") != 0) { // this loop reads the subchunk size then skips ahead to the next subchunk
        int size;
        if(read(fd, &size, 4) < 4) return 0; // current subchunk size is now in size
        lastHeader += size + 8; // +8 because subchunk header size does not include first 8 bytes (subchunk id and size)
        lseek(fd, lastHeader, SEEK_SET);
        if(read(fd, s, 4) < 4) return 0; // current subchunk id (name) is now in s
    }
    int size;
    if(read(fd, &size, 4) < 4) return 0; //file location is now at start of data
    return size;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Please provide path of WAV file and a file name\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    int fdOut = open(argv[2], O_WRONLY | O_CREAT, 0600);

    if(fd < 0) err();
    if(fdOut < 0) err();

    //creating the out file wav and copying everything from chunkID to DATA .wav metadata
    int dataSize = checkWav(fd);
    if(dataSize == 0) {
        printf("File provided does not appear to be in WAV format.\n");
        return 1;
    }
    unsigned char* bytes = malloc(dataSize);
    int readBytes;
    while( (readBytes = read(fd, bytes, dataSize)) ){
      if(readBytes < dataSize) {
          printf("WAV file broken: size mismatch\n");
          return 1;
      }
      write(fdOut, bytes, dataSize);
    }

    close(fd);
    close(fdOut);
}
