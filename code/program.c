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
    //printf("%d\n", lastHeader);
    return size;
}

// returns {sampleRate, bitsPerSample, dataSize}
int* checkWavMore(int fd) {
    int* result = malloc(3 * sizeof(int));
    char* s = malloc(5);
    if(read(fd, s, 4) < 4) return NULL;
    if(strcmp(s, "RIFF") != 0) return NULL;
    if(read(fd, s, 4) < 4) return NULL;
    //file size - 8 is stored in s now
    if(read(fd, s,
    4) < 4) return NULL;
    if(read(fd, s, 4) < 4) return 0; // next subchunk header is now in data
    int lastHeader = 12;
    while(strcmp(s, "fmt ") != 0) { // this loop reads the subchunk size then skips ahead to the next subchunk
        int size;
        if(read(fd, &size, 4) < 4) return 0; // current subchunk size is now in size
        lastHeader += size + 8; // +8 because subchunk header size does not include first 8 bytes (subchunk id and size)
        lseek(fd, lastHeader, SEEK_SET);
        if(read(fd, s, 4) < 4) return 0; // current subchunk id (name) is now in s
    }
    lseek(fd, 8, SEEK_CUR);
    if(read(fd, result, 4) < 4) return NULL;
    lseek(fd, 6, SEEK_CUR);
    short bps = 0;
    if(read(fd, &bps, 2) < 2) return NULL;
    result[1] = (int)bps;
    lastHeader = lseek(fd, 0, SEEK_CUR);
    if(read(fd, s, 4) < 4) return NULL;
    while(strcmp(s, "data") != 0) { // this loop reads the subchunk size then skips ahead to the next subchunk
        int size;
        if(read(fd, &size, 4) < 4) return 0; // current subchunk size is now in size
        lastHeader += size + 8; // +8 because subchunk header size does not include first 8 bytes (subchunk id and size)
        //printf("%d", lastHeader);
        lseek(fd, lastHeader, SEEK_SET);
        if(read(fd, s, 4) < 4) return 0; // current subchunk id (name) is now in s
    }
    if(read(fd, result + 2, 4) < 4) return NULL; //file location is now at start of data
    free(s);
    return result;
}

//Draw the first 100 amplitudes
void drawGraph(unsigned char* bytes, int dataSize) {
    short* curr = (short*)bytes;
    int scaled[100];
    for(int i = 0; i < dataSize / 2 && i < 100; ++i) {
        scaled[i] = *curr;
        scaled[i] = scaled[i] * 20 / 32767; //scaled is -20 to 20
        scaled[i] += 40;
        ++curr;
    }
    for(int i = 0; i < dataSize / 2 && i < 100; ++i) {
        int currScale = scaled[i];
        for(int j = 0; j < currScale; ++j) printf(" ");
        printf("*\n");
    }
}

void LSBinsert(unsigned char* bytes, int length, char* msg) {
    if(4 * strlen(msg) > length) {
        printf("WAV file provided is too small to store data\n");
        exit(1);
    }
    for(int i = 0; i <= strlen(msg); ++i) {
        int val;
        if(i == strlen(msg)) val = 0;
        else val = (int)(msg[i]);
        bytes[4*i] = leastSigBit(bytes[4*i], 2, (val & 192) >> 6);
        bytes[4*i+1] = leastSigBit(bytes[4*i+1], 2, (val & 48) >> 4);
        bytes[4*i+2] = leastSigBit(bytes[4*i+2], 2, (val & 12) >> 2);
        bytes[4*i+3] = leastSigBit(bytes[4*i+3], 2, (val & 3));
    }
}

void LSBextract(unsigned char* bytes) {
    int i = 1, pos = 0;
    while(i) {
        i = 0;
        i += (int)(bytes[pos]) & 3;
        i = i << 2;
        i += (int)(bytes[pos+1]) & 3;
        i = i << 2;
        i += (int)(bytes[pos+2]) & 3;
        i = i << 2;
        i += (int)(bytes[pos+3]) & 3;
        printf("%c", i);
        pos += 4;
    }
    printf("\n");
}

void freqInsert(unsigned char* bytes, char*msg, int freq, int sampleRate){

//2000hz samples per sec * 2 bytes per sample per channel * 2 channel -> 8000 bytes per sec
//every 8000th byte
//of a 4 point sin wave change the 2 and 4th. Change it for BOTH channels.

  int freqByteRate = sampleRate / freq;
  printf("bits sample channel: %d\n", sampleRate);
  printf("bytes rate: %d\n", freqByteRate);
  for (int n = 0; n <= strlen(msg); n++){
    bytes[(n * freqByteRate / 2) + (freqByteRate / 4)] = msg[n];
  }

}

void freqExtract(unsigned char* bytes, int length, int freq, int sampleRate){

  int freqByteRate = sampleRate / freq;
  printf("bits sample channel: %d\n", sampleRate);
  printf("bytes rate: %d\n", freqByteRate);
  for (int n = 0; n <= length; n++){

    printf("%c", bytes[(n * freqByteRate / 2) + (freqByteRate / 4)]);

  }
  printf("\n");


}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Please provide mode as an argument\n");
        return 1;
    }
    if(strcmp(argv[1], "encode") == 0) {
        if(argc < 4) {
            printf("ARGS should be \"[input file] [output file]\"\n");
            return 1;
        }
        //printf("%s\n", argv[2]);
        int fd = open(argv[2], O_RDONLY);
        int fdOut = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);

        if(fd < 0) err();
        if(fdOut < 0) err();

        //creating the out file wav and copying everything from chunkID to DATA .wav metadata
        int* a = checkWavMore(fd);
        if(a == NULL) {
            printf("File provided does not appear to be in WAV format.\n");
            return 1;
        }
        //doesn't work with the "JUNK" chunk in inst_test_mono and stereo
        //printf("%d %d\n", a[0], a[1]);
        int dataSize = a[2];

        unsigned char* bytes = malloc(dataSize);
        if(read(fd, bytes, dataSize) < dataSize) {
            printf("WAV file broken: data size incorrect");
            return 1;
        }
        LSBinsert(bytes, dataSize, "hello world");
        lseek(fd, 0, SEEK_SET);
        char buff[4];
        while( read(fd, buff, 2) ){
            write(fdOut, buff, 2);
            if(strncmp(buff, "ta", 2) == 0) break;
        }
        write(fdOut, &dataSize, 4);
        //printf("%d ", bytes[0]); printf("%d ", bytes[1]); printf("%d\n", bytes[2]);
        //printf("%d\n", lseek(fdOut, 0, SEEK_CUR));
        write(fdOut, bytes, dataSize);
        close(fd);
        close(fdOut);
        free(bytes);
    } else if(strcmp(argv[1], "decode") == 0) {
        if(argc < 3) {
            printf("ARGS should be \"[input file]\"\n");
            return 1;
        }
        int fd = open(argv[2], O_RDONLY);
        if(fd < 0) err();
        int dataSize = checkWav(fd);
        if(dataSize == 0) {
            printf("File provided does not appear to be in WAV format.\n");
            return 1;
        }
        unsigned char* bytes = malloc(dataSize);
        //printf("%d\n", lseek(fd, 0, SEEK_CUR));
        if(read(fd, bytes, dataSize) < dataSize) {
            printf("WAV broken\n");
            return 1;
        }
        //printf("%d ", bytes[0]); printf("%d ", bytes[1]); printf("%d\n", bytes[2]);
        LSBextract(bytes);
        close(fd);
        free(bytes);
    }
    else if(strcmp(argv[1], "freqEncode") == 0) {
      if(argc < 4) {
          printf("ARGS should be \"[input file] [output file]\"\n");
          return 1;
      }
      //printf("%s\n", argv[2]);
      int fd = open(argv[2], O_RDONLY);
      int fdOut = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);

      if(fd < 0) err();
      if(fdOut < 0) err();

      //creating the out file wav and copying everything from chunkID to DATA .wav metadata
      int* a = checkWavMore(fd);
      if(a == NULL) {
          printf("File provided does not appear to be in WAV format.\n");
          return 1;
      }
      //printf("%d %d\n", a[0], a[1]);
      int dataSize = a[2];

      unsigned char* bytes = malloc(dataSize);
      if(read(fd, bytes, dataSize) < dataSize) {
          printf("WAV file broken: data size incorrect");
          return 1;
      }
      freqInsert(bytes, "hello world", 2000, a[0]);
      lseek(fd, 0, SEEK_SET);
      char buff[4];
      while( read(fd, buff, 2) ){
          write(fdOut, buff, 2);
          if(strncmp(buff, "ta", 2) == 0) break;
      }
      write(fdOut, &dataSize, 4);
      //printf("%d ", bytes[0]); printf("%d ", bytes[1]); printf("%d\n", bytes[2]);
      //printf("%d\n", lseek(fdOut, 0, SEEK_CUR));
      write(fdOut, bytes, dataSize);
      close(fd);
      close(fdOut);
      free(bytes);
    }
    else if(strcmp(argv[1], "freqDecode") == 0) {
        if(argc < 3) {
            printf("ARGS should be \"[input file]\"\n");
            return 1;
        }
        int fd = open(argv[2], O_RDONLY);
        if(fd < 0) err();
        int* a = checkWavMore(fd);
        if(a == NULL) {
            printf("File provided does not appear to be in WAV format.\n");
            return 1;
        }
        //doesn't work with the "JUNK" chunk in inst_test_mono and stereo
        //printf("%d %d\n", a[0], a[1]);
        int dataSize = a[2];
        unsigned char* bytes = malloc(dataSize);
        //printf("%d\n", lseek(fd, 0, SEEK_CUR));
        if(read(fd, bytes, dataSize) < dataSize) {
            printf("WAV broken\n");
            return 1;
        }
        //printf("%d ", bytes[0]); printf("%d ", bytes[1]); printf("%d\n", bytes[2]);
        freqExtract(bytes, 11, 2000, a[0]);
        close(fd);
        free(bytes);
    }
    //drawGraph(bytes, dataSize);
}
