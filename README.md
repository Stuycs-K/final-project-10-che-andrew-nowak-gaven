[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ecp4su41)
## Group Info
Name: Noche (pronounced like mochi)  
Members: Andrew Che, Gaven Nowak
## Overview
This project is a tool that can be used to encrypt and decrypt information in WAV audio files. You can choose between multiple encryp\tion methods, all of which keep the audio nearly identical. The different tools are:  
- Least significant bit (LSB): Encodes the data into the 2 least significant bits of each audio sample
- Frequency-based: Encodes data into a wav file on a frequency basis.
- Data difference: Finds differences in the audio/data of two WAV files.
- Bit resample: Changes a WAV file's bits-per-sample.
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
```  
The example file names show that file's format and purpose. The .file extension means that file can be any file as long as its small enough to be stored in the WAV.  
The program will alert you if the file is not found, a WAV file seems to be misformatted, or you provided too few arguments.  

If you get a "WAV broken: size mismatch" error, that means the data size in the header of the WAV file you provided does not equal the real data size. If the WAV file was produced by our program, that means it was a bug of our program. If the WAV is from the internet/somewhere else, that means it was somehow incorrectly created.  
Some operations may destroy a WAV file. For example, frequency encoding at a very low frequency could render a WAV file unplayable. The program won't warn you if this happens, so if you make a WAV unplayable, try using a weaker encryption.


## VIDEO --> https://drive.google.com/file/d/1TycYg32cPtIPN7zl2JqHIzu_e1BYMsQn/view?usp=sharing

