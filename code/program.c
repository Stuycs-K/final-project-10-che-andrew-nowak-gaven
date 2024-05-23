#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
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

int main() {
    char* filePath = "test.wav";
    int fd = open(filePath, O_RDONLY);
    if(fd < 0) err();
    if(!checkWav(fd)) {
        printf("Error: Not WAVE format\n");
        return 1;
    }
    unsigned char c;
    if()
    while(read(fd, &c, 1)) { //keep reading byte by byte into c until no more bytes

    }
    return 0;
}