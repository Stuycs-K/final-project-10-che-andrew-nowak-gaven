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

## Andrew

### 5/23

Researched spectrogram and wav files, did some digging and found a sample wav > spectrogram program on StackOverflow

### 5/24

Deleted old program since we decided on a new idea. Made a program that extracts data bytes from a WAV file and puts it in an unsigned char (byte) array.

### 5/25-5/27

Wrote functions to insert and extract a string into/out of a WAV file's least significant bytes. Basically did our Processing image encoder and decoder but with WAV bytes instead of PNG bytes.

### 5/28

Separated encode and decode into two different targets and functions. Spent hours of homework time fixing a bug where I assumed the fmt subchunk was a multiple of 4 and then wrote a function to return sampleRate and bitsPerSample as well as dataSize.
