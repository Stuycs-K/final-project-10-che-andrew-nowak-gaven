[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ecp4su41)
## Group Info
Name: 
Members: Andrew Che, Gaven Nowak
## Overview
This project is a tool that can be used to encrypt and decrypt information in WAV audio files. You can choose between multiple encryp\tion methods, all of which keep the audio nearly identical. The different tools are:  
- Least significant bit (LSB): Encodes the data into the 2 least significant bits of each audio sample
- Frequency-based: Uses the WAV metadata to encode the data into an audio sample every second
- Data difference: Finds differences in the audio/data of two WAV files.
- Bit resample: Changes a WAV file from 16 bits per sample to 32 bits per sample.
- Third channel: Reads/writes data to the third channel of the audio data, which is not played on standard headphones and speakers.
## Instructions
All programs are run using the audioSteg file in the code subdirectory.  
Simply running the script will give you the flags and arguments for each option:  
```
final-project-10-che-andrew-nowak-gaven$ code/audioSteg
Please use the following commands:
LSB encode:        audioSteg -le raw.wav input.file encoded.wav
LSB decode:        audioSteg -ld encoded.wav output.file
freq encode:       audioSteg -fe raw.wav encoded.wav
freq decode:       audioSteg -fd encoded.wav
file diff:         audioSteg -df one.wav two.wav output.file
bit resample:      audioSteg -br 16bit.wav 32bit.wav
write 3rd channel: audioSteg -ce raw.wav
read 3rd channel:  audioSteg -cd encoded.wav
```  
The example file names show that file's format and purpose. The .file extension means that file can be any file as long as its small enough to be stored in the WAV.  
The program will alert you if the file is not found, a WAV file seems to be misformatted, or you provided too few arguments.  
If you get a "WAV broken: size mismatch" error, that means the data size in the header of the WAV file you provided does not equal the real data size. If the WAV file was produced by our program, that means it was a bug of our program. If the WAV is from the internet/somewhere else, that means it was somehow incorrectly created.