# Makefile for Assignemnt 5
# 15 May 2018
# Tony Guo
# GXXHEN001

samp: main.o
	g++ -o samp main.o -std=c++11

main.o: main.cpp  Normalize.h Audio.h
	g++ -c -o main.o main.cpp -std=c++11

run:
	./samp -r 44100 -b 8 -c 1 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 8 -c 2 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 16 -c 1 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o beezAddfrog -add beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 8 -c 1 -o beezCut -cut 2 9 beez18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o frogCut -cut 5 8 frogs18sec_44100_signed_16bit_stereo.raw
	./samp -r 44100 -b 8 -c 1 -o beezRaddfrog -radd 2 9 9 16 beez18sec_44100_signed_8bit_mono.raw frogs18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 16 -c 1 -o beezCatfrog -cat beez18sec_44100_signed_16bit_mono.raw frogs18sec_44100_signed_16bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o frogVolumeFactor -v 0.8 0.2 frogs18sec_44100_signed_16bit_stereo.raw
	./samp -r 44100 -b 16 -c 2 -o frogRev -rev frogs18sec_44100_signed_16bit_stereo.raw
	./samp -r 44100 -b 16 -c 2 -o frogRMS -rms frogs18sec_44100_signed_16bit_stereo.raw
	./samp -r 44100 -b 8 -c 1 -o beezNormalize -norm 50.0 50.0 beez18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 8 -c 1 -o beezFadeIn -fadein 4 beez18sec_44100_signed_8bit_mono.raw
	./samp -r 44100 -b 16 -c 2 -o beezFadeOut -fadeout 3 beez18sec_44100_signed_16bit_stereo.raw

test:
	g++ -c -o tests-main.o tests-main.cpp -std=c++11
	g++ -o tests tests-main.o tests-audio.cpp -std=c++11

runtest:
	./tests -s

clean:
	@rm -f *.o samp tests
	@rm -f beezAddfrog*.raw beezCut*.raw frogCut*.raw beezRadd*.raw beezCat*.raw
	@rm -f frogVolumeFactor*.raw frogRev*.raw beezNormalize*.raw beezFadeIn*.raw beezFadeOut*.raw
