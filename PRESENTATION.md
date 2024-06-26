## WAV file format
WAV is a format used to store uncompressed audio data. It's an application of the Microsoft RIFF format.  
The diagrams below illustrate an example of a standard (canonical) WAV file.  
![WAV file format diagram (small)](img/wavFormatSmall.png)
![WAV file format diagram (large)](img/wavFormatLarge.png)
### RIFF and Chunks
Chunks are a property of the RIFF format developed by Microsoft. At the beginning of a WAV file, the first four bytes have "RIFF" in ASCII, and four bytes later is "WAVE", which signals the start of a WAV(E) chunk. The WAV file format specifies two necessary subchunks:  
- The FMT chunk, which starts with "fmt " (space at the end) and includes metadata about the audio such as the number of channels, sample rate, and bits per sample.  
- The data chunk, which starts with "data" and contains the raw audio data, stored as amplitude values of the sound wave.

<!-- end of the list -->

There may be more subchunks in a WAV file that may serve any purpose. The only rule is that in every subchunk, the first four bytes are an id (name) and the four bytes after the ID have the size as a little-endian integer. This allows a program to traverse the subchunks looking for FMT and data.  
### Audio Data
Audio data is encoded as a series of numbers (samples) that represent the amplitudes of a sound wave at different times. There is nothing in the data that says how fast the samples should be played; all information needed to interpret the samples, such as sample rate and number of channels, are stored in the FMT subchunk.  
A sample's length (in bits) can be any multiple of 8, but the only ones commonly used are 8, 16, 24, and 32. The higher the bits per sample, the higher the range of possible amplitudes.  
The concurrent samples for different channels are next to each other in order. For example, a stereo (2-channel) WAV file's data would be left sample 1, right sample 1, left sample 2, right sample 2, etc.  
## Our Project
### LSB mode
This mode is similar to our image encoding lab, where we take the 2 least significant bits of each sample and encode data from a file in them. Unlike our image lab, you don't have to tell the program the size of the data; the data size is stored in the first 4 encrypted bits.  
This mode adapts to the bits per sample of the WAV file. For example, if a file is 24 bits per sample, it will encode every 3 bytes. It assumes the bytes are little endian, which follows the WAV file format specification.  
Encoding:  
Usage: ```code/audioSteg -le [WAV FILE] [DATA FILE] [OUTPUT WAV]```  
Example: ```code/audioSteg -le testing_sounds/tune.wav msg.txt out.wav```  
Decoding:  
Usage: ```code/audioSteg -ld [ENCODED WAV FILE] [OUTPUT FILE]```  
Example: ```code/audioSteg -ld out.wav out.txt```  
### Frequency mode
The data is encoded one byte per the sample rate/ input frequency, so it spreads it out over the whole audio at frequencies that you choose. The larger the frequency, the more data can be stored, but at the consequence of having more audio distortions.  
Encoding:  
Usage: ```code/audioSteg -fe [WAV FILE] [DATA FILE] [OUTPUT WAV] [FREQUENCY IN HZ]```  
Example: ```code/audioSteg -fe testing_sounds/tune.wav msg.txt out.wav 2000```  
Decoding:  
Usage: ```code/audioSteg -fd [ENCODED WAV FILE] [OUTPUT FILE] [FREQUENCY IN HZ]```  
Example: ```code/audioSteg -fd out.wav out.txt 2000```  
### WAV diff
This mode takes two WAV files and finds differences in their data. It outputs the different bytes into a file. The bytes may or may not be important depending on how it was encrypted, but the more important thing is the presence of a difference and how many bytes are different (which it tells you).  
Usage: ```code/audioSteg -df [WAV FILE 0] [WAV FILE 1] [OUTPUT FILE]```  
Example: ```code/audioSteg -df testing_sounds/tune.wav out.wav out.diffs```  
### Bit resample
This mode changes a WAV file's bits per sample in the metadata while leaving the actual data the same. This means audio players will interpret the bytes completely differently, jumbling the audio significantly. It can make an audio file unintelligible which can be used to hide a message in the audio itself.  
Usage: ```code/audioSteg -br [INPUT WAV] [OUTPUT WAV] [NEW SAMPLE RATE]```  
Example: ```code/audioSteg -br testing_sounds/tune.wav out.wav 32```  
### Channel encoding
This mode doesn't work, but the idea is to encode a message in the 3rd+ channel of a WAV file. Since most headphones and laptop speakers only play two channels, channels beyond the third one won't affect the audio.