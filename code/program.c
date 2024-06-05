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


/*
//takes 'byte' and zeros out all least significant bits up to and including 'num' and inserts 'insert' into those bits.
unsigned char leastSigBit(unsigned char byte, int num, int insert){
  return byte - (byte % ((int) pow(2,num))) + insert;
}
*/

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

void LSBinsert(unsigned char* bytes, int length, unsigned char* msg, int msgLength) {
    if(4 * msgLength > length) {
        printf("WAV file provided is too small to store data\n");
        exit(1);
    }
    //printf("b%d\n", *(int*)msg);
    for(int i = 0; i < msgLength; ++i) {
        unsigned char val = msg[i];
        bytes[4*i] = (bytes[4*i] & (!3)) + (val >> 6 & 3);
        bytes[4*i+1] = (bytes[4*i+1] & (!3)) + (val >> 4 & 3);
        bytes[4*i+2] = (bytes[4*i+2] & (!3)) + (val >> 2 & 3);
        bytes[4*i+3] = (bytes[4*i+3] & (!3)) + (val & 3);
    }
}

int LSBextract(unsigned char* bytes, unsigned char** m) {
    unsigned char csize[4];
    for(int i = 0; i < 4; ++i) {
        csize[i] = 0;
        csize[i] += bytes[4*i] & 3;
        csize[i] = csize[i] << 2;
        csize[i] += bytes[4*i+1] & 3;
        csize[i] = csize[i] << 2;
        csize[i] += bytes[4*i+2] & 3;
        csize[i] = csize[i] << 2;
        csize[i] += bytes[4*i+3] & 3;
    }
    int size = *(int*)csize;
    size -= sizeof(int);
    printf("%d", size);
    fflush(stdout);
    *m = malloc(size);
    unsigned char* msg = *m;
    for(int i = 0; i < size; ++i) {
        msg[i] = 0;
        msg[i] += bytes[4*i+16] & 3;
        msg[i] = msg[i] << 2;
        msg[i] += bytes[4*i+17] & 3;
        msg[i] = msg[i] << 2;
        msg[i] += bytes[4*i+18] & 3;
        msg[i] = msg[i] << 2;
        msg[i] += bytes[4*i+19] & 3;
    }
    return size;
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

//'d' for difference mode  ->  returns the difference of the two file bytes
//'l' for literal mode  ->  returns the new value if the new value is different than the original
int bytesDiff(unsigned char* orig, unsigned char* new, int length, char mode, unsigned char* diffs){
  unsigned char difference;
  int diffsIndex = 0;

  for(int n = 0; n < length; n++){
    difference = abs(orig[n] - new[n]);
    if(difference == 0){
      continue;
    }

    if(mode == 'd'){
      //printf("%x", difference);
      diffs[diffsIndex] = difference;
      diffsIndex++;
    }
    else if(mode == 'l'){
      //printf("%x", new[n]);
      diffs[diffsIndex] = new[n];
      diffsIndex++;
    }
  }
  //printf("\n");
  return diffsIndex;
}

int fileToBytes(int fd, unsigned char** bytes) {
    int size = lseek(fd, 0, SEEK_END) + sizeof(int);
    *bytes = malloc(size);
    lseek(fd, 0, SEEK_SET);
    printf("%d", size);
    memcpy(*bytes, &size, sizeof(int));
    //printf("a%d\n", **(int**)bytes);
    if(read(fd, *bytes + sizeof(int), size) < 0) err();
    lseek(fd, 0, SEEK_SET);
    return size;
}


//fd needs read and write
void bitResample(int fd, char mode, unsigned short newBitsPerSample){
  // for(int n = 0; n < 200; n++){
  //   printf("%d: %x, new: %hu\n", n, bytes[n], bitsPerSample);
  // }



  unsigned short bitSampleRate = 0;
  lseek(fd, 34, SEEK_SET);
  read(fd, &bitSampleRate, sizeof(unsigned short));

  printf("OG BIT RATE: %hu\n", bitSampleRate);
  printf("NEW BIT RATE: %hu\n", newBitsPerSample);

  if(bitSampleRate == 0){
    printf("\n\nWARNING BITSAMPLERATE == 0 in BIT REAMPLE NOT GOOD!\n\n\n");
    return;
  }

  float ratio = newBitsPerSample / bitSampleRate;
  write(fd, &newBitsPerSample, sizeof(unsigned short));

  int byteRate;
  lseek(fd, 28, SEEK_SET);
  read(fd, &byteRate, sizeof(int));
  printf("OG BYTE RATE: %d\n", byteRate);

  byteRate *= ratio;
  write(fd, &byteRate, sizeof(int));
  printf("NEW BYTE RATE: %d\n", byteRate);



  return;
}


int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Please provide mode as an argument\n");
        return 1;
    }
    if(strcmp(argv[1], "encode") == 0) {
        if(argc < 5) {
            printf("ARGS should be \"[input wav] [input file] [output wav]\"\n");
            return 1;
        }
        //printf("%s\n", argv[2]);
        int fd = open(argv[2], O_RDONLY);
        int fdMsg = open(argv[3], O_RDONLY);
        int fdOut = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0600);

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
        unsigned char* msg;
        int msgSize = fileToBytes(fdMsg, &msg);
        LSBinsert(bytes, dataSize, msg, msgSize);
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
        close(fdMsg);
        close(fdOut);
        free(bytes);
        //free(msg);
    } else if(strcmp(argv[1], "decode") == 0) {
        if(argc < 4) {
            printf("ARGS should be \"[input wav] [output file]\"\n");
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
        unsigned char* msg;
        int msgSize = LSBextract(bytes, &msg);
        int fdOut = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, 0600);
        write(fdOut, msg, msgSize);
        close(fdOut);
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
    else if(strcmp(argv[1], "diff") == 0) {
      if(argc < 5) {
          printf("ARGS should be \"[original file] [new file] [output file]\"\n");
          return 1;
      }
      //printf("%s\n", argv[2]);
      int fdOrig = open(argv[2], O_RDONLY);
      int fdNew = open(argv[3], O_RDONLY);
      int fdOut = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0600);

      if(fdOrig < 0) err();
      if(fdOut < 0) err();

      //creating the out file wav and copying everything from chunkID to DATA .wav metadata
      int* a = checkWavMore(fdOrig);
      if(a == NULL) {
          printf("File provided does not appear to be in WAV format (orig).\n");
          return 1;
      }
      int* b = checkWavMore(fdNew);
      if(b == NULL) {
          printf("File provided does not appear to be in WAV format (new).\n");
          return 1;
      }
      //printf("%d %d\n", a[0], a[1]);
      int dataSizeOrig = a[2];
      int dataSizeNew = b[2];

      unsigned char* bytesOrig = malloc(dataSizeOrig);
      unsigned char* bytesNew = malloc(dataSizeNew);

      if(read(fdOrig, bytesOrig, dataSizeOrig) < dataSizeOrig) {
          printf("WAV file broken: data size incorrect");
          return 1;
      }
      if(read(fdNew, bytesNew, dataSizeNew) < dataSizeNew) {
          printf("WAV file broken: data size incorrect");
          return 1;
      }

      unsigned char* diffs = malloc(dataSizeOrig);
      int diffSize;
      diffSize = bytesDiff(bytesOrig, bytesNew, dataSizeOrig, 'l', diffs);
      printf("Size of diff: %d\n", diffSize);



      write(fdOut, &diffs[0], diffSize);
      close(fdNew);
      close(fdOrig);
      close(fdOut);
      free(bytesOrig);
      free(bytesNew);
      free(diffs);
    }


    else if(strcmp(argv[1], "bitResample") == 0) {
      if(argc < 4) {
          printf("ARGS should be \"[original file] [output file]\"\n");
          return 1;
      }
      //printf("%s\n", argv[2]);
      int fd = open(argv[2], O_RDWR);
      int fdOut = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, 0600);

      if(fd < 0) err();
      if(fdOut < 0) err();


      char* bytes = malloc(sizeof(char));
      while(read(fd, bytes, sizeof(char))){
        write(fdOut, bytes, sizeof(char));
      }


      bitResample(fdOut, 'l', 32);

      close(fd);
      close(fdOut);
      free(bytes);
    }
}



/*

To do:

- Incporate LSB into freqEncode and insertion into encode

- Create a 'resample' function because we can encode information that is usually 32 bit into 16 bit or vice versa
  (similar to the Asian character ASCII 32/16 bit encoding)
   ^  Should have a 'literal' mode and a 'adjust' mode: Literal just smashes the bytes together but recompile will re-adjust
      the values to be consistant with the orignal .wav

- Create a shift polarity function that can shift a polarity of a sound on one channel

- figure out if we can do a spectogram image hider but do it in C somehow

- create a channel ecoded thing by shoving data into channels that don't exist usually (like 12 channels)





*/
