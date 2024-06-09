# Work Log

## Gaven

### 5/22

Researched and tested spectogram audio analysis programs with Andrew. Also researched the components and metadata of a .wav file.

### 5/23

More research and more testing of the spectogram audio analysis programs with Andrew. Starting to implement byte editing of the data that controls the sound of the .wav file.

### 5/24

Scrapping the spectogram idea and will now focus on performing LSB on a .wav file.

### 5/25-5/27

Helped Andrew write functions to insert and extract a string into/out of a WAV file's least significant bytes. Basically did our Processing image encoder and decoder but with WAV bytes instead of PNG bytes. Also researching an experimental frequency method of doing LSB on the audio file.

### 5/28

Continuing to research the frequency method and beginning to implement freqEncode/Decode using Andrew's work on getting bitsPerSample.

### 5/29

Continuing to work on freqEncode and Decode. freqEncode works for the 480hz file on the 240 band. Also cleaning up main.

### 5/30

Continuing to work on freqEncode and Decode. Helping Andrew debug lsb insert

### 5/31

freqEncode and decode working.

### 6/2

byte diff function working also made makefile get rid of temp output files

### 6/3

Working on a 'resample' function to help hide data via the combination/seperation of bytes in the bit sample rate

### 6/4
Continuing resample. still doesn't work but good progress

### 6/5
resample almost done and working on channelRead and write


## Andrew

### 5/23

Researched spectrogram and wav files, did some digging and found a sample wav > spectrogram program on StackOverflow

### 5/24

Deleted old program since we decided on a new idea. Made a program that extracts data bytes from a WAV file and puts it in an unsigned char (byte) array.

### 5/25-5/27

Wrote functions to insert and extract a string into/out of a WAV file's least significant bytes. Basically did our Processing image encoder and decoder but with WAV bytes instead of PNG bytes.

### 5/28

Separated encode and decode into two different targets and functions. Spent hours of homework time fixing a bug where I assumed the fmt subchunk was a multiple of 4 and then wrote a function to return sampleRate and bitsPerSample as well as dataSize.

### 5/29

Wrote a function for Gaven to get info about the wave. Fixed a bug where it assumed the fmt subchunk was first. Spent all homework time trying to fix LSBextract, couldn't do it.

### 5/30

Fixed two bugs with LSBextract, encoding and decoding now technically work but ruin the audio. I think it's because it turns 0s into non-0s so I tried making it only change non-0s but couldn't figure it out.

### 5/31

Gaven told me in class the audio is ruined because I'm encoding in every byte. Each sample is 2 bytes so changing the more significant byte is ruining the audio. I'm working on fixing it but it's hard because my loops rely on every byte being encoded.

### 6/1-6/3

Very slow progress on fixing LSB. I took a while worrying about the int size being little/big endian and if that would affect the decoding. I don't know what was going wrong but I kept changing and rewriting until the correct size was printed, but now the data isn't being extracted correctly. Getting closer though.

### 6/4

Didn't work on it much on the day off. Added and removed a lot of debug prints but no commits and I still don't know what's wrong.

### 6/5

Finally finished fixing the LSB encoding. Audio is still actually a bit static-y but much much better. The last bug was me starting the data loop a byte too early which threw off all the bytes.

### 6/6-6/7

Added some stuff to help Gaven: a changeChannels function and checkWavMore now also gives numChannels. I also started on a bash script to make it easier to run different commands.

### 6/8-6/9

I finished the bash script and made it work outside of the code directory. I also made freqInsert and Extract take files to encode instead of just "hello world". I also worked on the readme and presentation files.