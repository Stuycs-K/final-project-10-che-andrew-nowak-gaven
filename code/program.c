#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>


int err() {
    printf("Error %d: %s\n", errno, strerror(errno));
    exit(1);
}

//assumes fd is at start of file, returns 0 if not WAV and 1 if WAV
int checkWav(int fd) {
    char* s = malloc(5);
    if(read(fd, s, 4) < 4) return 0;

}


//takes 'byte' and zeros out all least significant bits up to and including 'num' and inserts 'insert' into those bits.
unsigned char leastSigBit(unsigned char byte, int num, int insert){
  return byte - (byte % ((int) pow(2,num))) + insert;
}


int main(int argc, char *argv[]) {
    // char* filePath = "test.wav";
    // int fd = open(filePath, O_RDONLY);
    // if(fd < 0) err();
    // if(!checkWav(fd)) {
    //     printf("Error: Not WAVE format\n");
    //     return 1;
    // }
    // unsigned char c;
    // if()
    // while(read(fd, &c, 1)) { //keep reading byte by byte into c until no more bytes
    //
    // }

    if( ( argc > 0 ) || (strcmp(argv[1], "encode") == 0 )) {
      printf("yay!\n");
    }
    unsigned char byte = 95;
    printf("%d\n", leastSigBit(byte, 4, 7));
    return 0;
}
