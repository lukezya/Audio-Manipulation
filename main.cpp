#include <iostream>
#include <string>
#include "Audio.h"

using namespace std;

int main(int argc, char * argv[]) {
    //samp -r sampleRateInHz -b bitCount -c noChannels [-o outFileName ] [<ops>] soundFile1 [soundFile2]
    int iSampleRateInHz = atoi(argv[2]);
    int iBitCount = atoi(argv[4]);   //check if correct type
    int iNoChannels = atoi(argv[6]);

    //check if -outFileName is supplied
    int iTrack = 7;
    string sOutFileName = "out";
    string sOut = string(argv[7]);
    if (sOut=="-o"){
        sOutFileName = string(argv[8]);
        iTrack += 2;
    }

    //operations
    string sOperation = string(argv[iTrack]);
    if (sOperation=="-add"){
        cout << "Add operation in progress..." << endl;
        //-add: add soundFile1 and soundFile2
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);
        ++iTrack;
        string sSoundFile2 = string(argv[iTrack]);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            Audio<int8_t, MONO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int8_t, MONO> aOut = a1+a2;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            Audio<int8_t, STEREO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int8_t, STEREO> aOut = a1+a2;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            Audio<int16_t, MONO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int16_t, MONO> aOut = a1+a2;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            Audio<int16_t, STEREO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int16_t, STEREO> aOut = a1+a2;
            aOut.save(sOutFileName);
        }

        cout << "Add operation complete" << endl;

    } else if (sOperation=="-cut") {
        cout << "Cut operation in progress..." << endl;
        //-cut r1 r2: remove samples over range [r1,r2] (inclusive) (assumes one sound file)
        ++iTrack;
        int iRemoveBegin = atoi(argv[iTrack]);
        ++iTrack;
        int iRemoveEnd = atoi(argv[iTrack]);
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);
        pair<int, int> F = make_pair(iRemoveBegin, iRemoveEnd);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, MONO> aOut = a1^F;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, STEREO> aOut = a1^F;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, MONO> aOut = a1^F;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, STEREO> aOut = a1^F;
            aOut.save(sOutFileName);
        }
        cout << "Cut operation complete" << endl;

    } else if (sOperation=="-radd") {
        cout << "Ranged Add operation in progress..." << endl;
        //-radd r1 r2 s1 s2 : add soundFile1 and soundFile2 over sub-ranges indicated (in seconds) - the ranges must be equal in length
        ++iTrack;
        int iSound1Begin = atoi(argv[iTrack]);
        ++iTrack;
        int iSound1End = atoi(argv[iTrack]);
        ++iTrack;
        int iSound2Begin = atoi(argv[iTrack]);
        ++iTrack;
        int iSound2End = atoi(argv[iTrack]);
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);
        ++iTrack;
        string sSoundFile2 = string(argv[iTrack]);
        pair<int, int> range1 = make_pair(iSound1Begin, iSound1End);
        pair<int, int> range2 = make_pair(iSound2Begin, iSound2End);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            Audio<int8_t, MONO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int8_t, MONO> aOut = a1.rangedAdd(range1, range2, a2);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            Audio<int8_t, STEREO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int8_t, STEREO> aOut = a1.rangedAdd(range1, range2, a2);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            Audio<int16_t, MONO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int16_t, MONO> aOut = a1.rangedAdd(range1, range2, a2);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            Audio<int16_t, STEREO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int16_t, STEREO> aOut = a1.rangedAdd(range1, range2, a2);
            aOut.save(sOutFileName);
        }

        cout << "Ranged Add operation complete" << endl;

    } else if (sOperation=="-cat") {
        cout << "Concatenate operation in progress..." << endl;
        //-cat: concatenate soundFile1 and soundFile2
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);
        ++iTrack;
        string sSoundFile2 = string(argv[iTrack]);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            Audio<int8_t, MONO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int8_t, MONO> aOut = a1|a2;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            Audio<int8_t, STEREO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int8_t, STEREO> aOut = a1|a2;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            Audio<int16_t, MONO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int16_t, MONO> aOut = a1|a2;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            Audio<int16_t, STEREO>a2(iSampleRateInHz);
            a1.load(sSoundFile1);
            a2.load(sSoundFile2);
            Audio<int16_t, STEREO> aOut = a1|a2;
            aOut.save(sOutFileName);
        }

        cout << "Concatenate operation complete" << endl;

    } else if (sOperation=="-v") {
        cout << "Volume Factor operation in progress..." << endl;
        //-v r1 r2: volume factor for left/right audio (def=1.0/1.0) (assumes one sound file)
        ++iTrack;
        float fLeftVolume = atof(argv[iTrack]);
        ++iTrack;
        float fRightVolume = atof(argv[iTrack]);
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);
        pair<float, float> volumeFactor = make_pair(fLeftVolume, fRightVolume);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, MONO> aOut = a1*volumeFactor;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, STEREO> aOut = a1*volumeFactor;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, MONO> aOut = a1*volumeFactor;
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, STEREO> aOut = a1*volumeFactor;
            aOut.save(sOutFileName);
        }

        cout << "Volume Factor operation complete" << endl;

    } else if (sOperation=="-rev") {
        cout << "Reverse operation in progress..." << endl;
        //-rev: reverse sound file (assumes one sound file only)
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, MONO> aOut = a1.reverse();
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, STEREO> aOut = a1.reverse();
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, MONO> aOut = a1.reverse();
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, STEREO> aOut = a1.reverse();
            aOut.save(sOutFileName);
        }

        cout << "Reverse operation complete" << endl;

    } else if (sOperation=="-rms") {
        cout << "RMS operation in progress..." << endl;
        //-rms: prints out the RMS of the sound file (assumes one sound file only)
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            float fOut = a1.computeRMS();
            cout << "RMS (1 Channel): " << fOut << endl;
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            pair<float, float> fOut = a1.computeRMS();
            cout << "RMS (2 Channels) Left: " << (float)fOut.first << " Right: " << (float)fOut.second << endl;
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            float fOut = a1.computeRMS();
            cout << "RMS (1 Channel): " << fOut << endl;
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            pair<float, float> fOut = a1.computeRMS();
            cout << "RMS (2 Channels) Left: " << fOut.first << " Right: " << fOut.second << endl;
        }

        cout << "RMS operation complete" << endl;


    } else if (sOperation=="-norm") {
        cout << "Normalize operation in progress..." << endl;
        //-norm r1 r2: normalize file for left/right audio (assumes one sound file only and that r1 and r2 are floating point RMS values)
        ++iTrack;
        float fLeftRMS = atof(argv[iTrack]);
        ++iTrack;
        float fRightRMS = atof(argv[iTrack]);
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);
        pair<float,float> desiredRMS = make_pair(fLeftRMS, fRightRMS);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, MONO> aOut = a1.normalizeSound(desiredRMS);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, STEREO> aOut = a1.reverse();
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, MONO> aOut = a1.normalizeSound(desiredRMS);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, STEREO> aOut = a1.normalizeSound(desiredRMS);
            aOut.save(sOutFileName);
        }

        cout << "Normalize operation complete" << endl;


    } else if (sOperation=="-fadein") {
        cout << "Fade In operation in progress..." << endl;
        //-fadein n: n is the number of seconds (floating point number) to slowly increase the volume (from 0) at the start of soundFile1 (assumes one sound file)
        ++iTrack;
        float fNoSec = atof(argv[iTrack]);
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, MONO> aOut = a1.fadeIn(fNoSec);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, STEREO> aOut = a1.fadeIn(fNoSec);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, MONO> aOut = a1.fadeIn(fNoSec);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, STEREO> aOut = a1.fadeIn(fNoSec);
            aOut.save(sOutFileName);
        }

        cout << "Fade In operation complete" << endl;

    } else if (sOperation=="-fadeout") {
        cout << "Fade Out operation in progress..." << endl;
        //-fadeout n: n is the number of seconds (floating point number) to slowly decrease the volume (from 1.0 to 0) at the end of soundFile1 (assumes one sound file)
        ++iTrack;
        float fNoSec = atof(argv[iTrack]);
        ++iTrack;
        string sSoundFile1 = string(argv[iTrack]);

        if (iBitCount==8 && iNoChannels==1){
            Audio<int8_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, MONO> aOut = a1.fadeOut(fNoSec);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==8 && iNoChannels==2){
            Audio<int8_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int8_t, STEREO> aOut = a1.fadeOut(fNoSec);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==1){
            Audio<int16_t, MONO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, MONO> aOut = a1.fadeOut(fNoSec);
            aOut.save(sOutFileName);
        }

        else if (iBitCount==16 && iNoChannels==2){
            Audio<int16_t, STEREO>a1(iSampleRateInHz);
            a1.load(sSoundFile1);
            Audio<int16_t, STEREO> aOut = a1.fadeOut(fNoSec);
            aOut.save(sOutFileName);
        }

        cout << "Fade Out operation complete" << endl;

    } else {
        cout << "Your command construct for program samp was incorrect." << endl;
    }


    return 0;
}
