-----------------------------------
Files in this directory
-----------------------------------
sample_input/ folder - contains filter audio files to use for samp program.

Audio.h - templated Audio class with its methods and data.

Normalize.h - templated custom functor for std::transform in normalizeSound operation.

main.cpp - driver source file for Simple Audio Manipulation Program (SAMP) and contains the
main() method.

Makefile - makefile to compile source files and get: samp and main.o

tests-main.cpp - main unit test file that define CATCH_CONFIG_MAIN for the rest
of the test files.

tests-audio.cpp - unit test for Audio containing tests for all
operations: overloaded operators, audio transformation operators, extra credit.

catch.hpp - library used in test classes for testing.

*.raw - raw sound files used for testing and generating sound files/clips.

-----------------------------------
Running samp
-----------------------------------
Open this directory in terminal.

Run the following commands:
make clean
make
make run

*make run will run the executable imageops 14 times:
1) Add operation:
  ./samp -r 44100 -b 8 -c 1 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
  ./samp -r 44100 -b 8 -c 2 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
  ./samp -r 44100 -b 16 -c 1 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
  ./samp -r 44100 -b 16 -c 2 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
2) Cut operation:
  ./samp -r 44100 -b 8 -c 1 -o beezCut -cut 2 9 beez18sec_44100_signed_8bit_mono.raw
  ./samp -r 44100 -b 16 -c 2 -o frogCut -cut 5 8 frogs18sec_44100_signed_16bit_stereo.raw
3) Ranged Add operation:
  ./samp -r 44100 -b 8 -c 1 -o beezRaddfrog -radd 2 9 9 16 beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
4) Catenate operation:
  ./samp -r 44100 -b 16 -c 1 -o beezCatfrog -cat beez18sec_44100_signed_16bit_mono.raw frogs18sec_44100_signed_16bit_mono.raw
5) Volume Factor operation:
  ./samp -r 44100 -b 16 -c 2 -o frogVolumeFactor -v 0.8 0.2 frogs18sec_44100_signed_16bit_stereo.raw
5) Reverse operation:
  ./samp -r 44100 -b 16 -c 2 -o frogRev -rev frogs18sec_44100_signed_16bit_stereo.raw
6) Compute RMS operation:
  ./samp -r 44100 -b 16 -c 2 -o frogRMS -rms frogs18sec_44100_signed_16bit_stereo.raw
7) Normalize operation:
  ./samp -r 44100 -b 8 -c 1 -o beezNormalize -norm 50.0 50.0 beez18sec_44100_signed_8bit_mono.raw
8) Fade In operation:
  ./samp -r 44100 -b 8 -c 1 -o beezFadeIn -fadein 4 beez18sec_44100_signed_8bit_mono.raw
9) Fade Out operation:
  ./samp -r 44100 -b 16 -c 2 -o beezFadeOut -fadeout 3 beez18sec_44100_signed_16bit_stereo.raw
---The above values for the operation arguments were used for demo runs---

The following files will be created:
1) Add operation:
  4 beezAddfrog*.raw files
2) Cut operation:
  2 frogCut*.raw files
3) Ranged Add operation:
  1 beezRaddfrog*.raw file
4) Catenate operation:
  1 beezCatfrog*.raw file
5) Volume Factor operation:
  1 frogVolumeFactor*.raw file
5) Reverse operation:
  1 frogRev*.raw file
6) Compute RMS operation:
  1 frogRMS*.raw file
7) Normalize operation:
  1 beezNormalize*.raw file
8) Fade In operation:
  1 beezFadeIn*.raw file
9) Fade Out operation:
  1 beezFadeOut*.raw file

-----------------------------------
Checking outputted files
-----------------------------------
Open this directory in terminal.

Run the following command:
play -r <sampleRate> -e signed -b <bitCount> -c <noChannels> <OutputFile>

The <sampleRate>, <bitCount>, and <noChannels> can all be gotten from the name
of <OutputFile>

-----------------------------------
Running unit tests
-----------------------------------
Open this directory in terminal.

After running the commands above in "Running imageops",
Run the following commands:
make test
make runtest

The file "tests" will be created, and make runtest runs this executable.
The results of the unit tests will be outputted to console.

-----------------------------------
Running samp with own arguments
-----------------------------------
Open terminal in this directory.

To run volimage with own arguments, type in a command in the following form:
./samp -r <sampleRateInHz> -b <bitCount> -c <noChannels> [-o outFileName ] [<ops>] soundFile1 [soundFile2]

*[<ops>] can be any of the following options: (soundFile1 and soundFile2 represent raw sound filenames)
  -add: add soundFile1 and soundFile2
  -cut r1 r2: remove samples over range [r1,r2]
  -radd r1 r2 s1 s2 : add sub-ranges (in seconds), ranges must be equal in length
  -cat: concatenate soundFile1 and soundFile2
  -v r1 r2: volume factor for left/right audio
  -rev: reverse sound file
  -rms: Prints out the RMS of the sound file
  -norm r1 r2: normalize file for left/right audio
  -fadein n: slowly increase the volume (from 0) at the start of soundFile1
  -fadeout n: slowly decrease the volume (from 1.0 to 0) at the end of soundFile1

To run this command with available test data, copy the sound files from sample_input/
folder into this directory first before using them.
