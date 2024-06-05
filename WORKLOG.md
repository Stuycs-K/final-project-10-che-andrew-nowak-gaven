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

### 5/31-6/5

Spent this whole time making the audio not get ruined. The bug was caused by modifying every byte instead of every 4 bytes, so the 4 byte int values in the audio changed a lot. When I tried to do every 4 bytes, it caused a lot of errors. I had to write the 4-bit int for size in manually (should work if computer is little or big endian) and debug a bunch of miswritten array stuff. Debugging took a lot of time but I finally finished in 6/5.