#ifndef ASSIGNMENT5_AUDIO_H
#define ASSIGNMENT5_AUDIO_H


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include "Normalize.h"

enum Channel{
    MONO=1,
    STEREO=2
};

template <typename T, Channel channel = MONO>
class Audio {
private:
    std::vector<T> data;
    int sampleRate;
    int bitCount;
    int noSamples;
    float length;
public:
    /*//Audio iterator for data vector
    class iterator {
        friend class Audio;
    private:
        T *ptr;

        //construct only via Image class (begin/end)
        iterator (T *p):ptr(p){

        }

    public:
        //copy construct is public
        iterator(const iterator & other):ptr(other.ptr){

        }

        //define overloaded ops: *, ++, --, =
        T& operator*(void){
            return *ptr;
        }

        void operator++(void){
            ++ptr;
        }

        void operator--(void){
            --ptr;
        }

        iterator& operator=(const iterator &rhs){
            ptr = rhs.ptr;
            return *this;
        }

        //other methods for iterator
        bool operator!=(const iterator& rhs)const{
            if (ptr!=rhs.ptr)
                return true;
            else
                return false;
        }

        bool operator==(const iterator& rhs)const{
            if (ptr==rhs.ptr)
                return true;
            else
                return false;
        }

        iterator& operator+(const int& moveBy){
            ptr = ptr+moveBy;
            return *this;
        }
    };*/

    //default constructor
    Audio(int sampleRateInHz=0):sampleRate(sampleRateInHz), noSamples(0), length(0){
        if (sizeof(T)==sizeof(int8_t))
            bitCount=8;
        else if (sizeof(T)==sizeof(int16_t))
            bitCount=16;
    }

    //parameterized constructor
    Audio(int SampleRate, int BitCount, int NoSamples):sampleRate(SampleRate), bitCount(BitCount), noSamples(NoSamples){
        data.resize(noSamples);
        length = noSamples/(float) sampleRate;
    }

    //test constructor
    Audio(int SampleRate, int BitCount, int NoSamples, std::initializer_list<unsigned char> lst):sampleRate(SampleRate), bitCount(BitCount), noSamples(NoSamples){
        data.resize(noSamples);
        auto onSample = lst.begin();
        for (int i=0;i<noSamples;++i){
            data[i] = *onSample;
            ++onSample;
        }
        length = noSamples/(float) sampleRate;
    }

    std::vector<T> getBuffer() {
        return data;
    }
    /*Image::Image(const int &w, const int &h, std::initializer_list<unsigned char> lst):width(w), height(h) {
        data=unique_ptr<unsigned char[]>(new unsigned char[width*height]);
        //copy(lst.begin(), lst.end(), this->begin());
        auto onImage = lst.begin();
        for(int i=0;i<width*height;++i){
            data[i]= *onImage;
            ++onImage;
        }
    }*/

    //destructor
    ~Audio(){
        data.clear();
    }

    //copy constructor
    Audio(const Audio& other):sampleRate(other.sampleRate), bitCount(other.bitCount), noSamples(other.noSamples), length(other.length), data(other.data){

    }

    //move constructor
    Audio(Audio&& other):sampleRate(other.sampleRate), bitCount(other.bitCount), noSamples(other.noSamples), length(other.length), data(std::move(other.data)){
        //zero out values
        other.sampleRate = 0;
        other.bitCount = 0;
        other.noSamples = 0;
        other.length = 0;
    }

    //copy assignment operator
    Audio& operator=(const Audio& rhs){
        //release
        std::vector<T> empty;
        data.swap(empty);

        //acquire and assign
        data = rhs.data;
        sampleRate = rhs.sampleRate;
        bitCount = rhs.bitCount;
        noSamples = rhs.noSamples;
        length = rhs.length;

        return *this;
    }

    //move assignment operator
    Audio& operator=(Audio&& rhs){
        //release
        std::vector<T> empty;
        data.swap(empty);

        //acquire resources
        data = std::move(rhs.data);
        data = rhs.data;
        sampleRate = rhs.sampleRate;
        bitCount = rhs.bitCount;
        noSamples = rhs.noSamples;
        length = rhs.length;

        //zero out values
        rhs.sampleRate = 0;
        rhs.bitCount = 0;
        rhs.noSamples = 0;
        rhs.length = 0;

        return *this;
    }

    //overloaded operations
    //A|B: concatenate audio file A and B
    Audio operator|(const Audio& rhs) const{
        Audio result(*this);
        result.data.resize(rhs.data.size()+data.size());
        for (int i = data.size(); i < result.data.size(); ++i) {
            result.data[i] = rhs.data[i-data.size()];
        }
        return result;
    }
    //A*F: volume factor A with F
    Audio operator*(const std::pair<float,float>& volumeFactors)const{
        Audio result(*this);
        //std::cout << volumeFactors.first << std::endl;
        for (int i = 0; i < result.data.size(); ++i) {
            result.data[i] *= volumeFactors.first;
            //std::cout << result.data[i] << std::endl;
        }
        return result;
    }

    //A+B: add sound file amplitudes together (per sample)
    Audio operator+(const Audio& rhs) const{
        Audio result(*this);

        int max;
        if (bitCount==8)
            max = INT8_MAX;
        else if (bitCount==16)
            max = INT16_MAX;

        for (int i=0;i<data.size();++i){
            if (data[i]+rhs.data[i]>max)
                result.data[i] = max;
            else
                result.data[i] = data[i]+rhs.data[i];
        }

        return result;
    }

    //A^F: F will be a std::pair<int,int> specifies cut range to be cut from sound file A
    Audio operator^(const std::pair<int,int>& cutRange)const{
        Audio result(*this);

        int sampleCutBegin = cutRange.first*sampleRate;
        int sampleCutEnd = cutRange.second*sampleRate;
        result.data.erase(result.data.begin()+sampleCutBegin, result.data.begin()+sampleCutEnd);

        return result;
    }

    //audio transformation
    //Reverse: reverse all samples
    Audio reverse() const {
        Audio result(*this);
        std::reverse(result.data.begin(),result.data.end());
        return result;
    }

    //Ranged add: select two (same length) sample ranges from two signals and add them together
    Audio rangedAdd(const std::pair<int,int>& cutRange1, const std::pair<int,int>& cutRange2, const Audio& other)const{
        //get sample ranges
        int sampleCutBegin1 = cutRange1.first*sampleRate;
        int sampleCutEnd1 = cutRange1.second*sampleRate;
        Audio cut1(sampleRate, bitCount, sampleCutEnd1-sampleCutBegin1);
        std::copy(data.begin()+sampleCutBegin1, data.begin()+sampleCutEnd1, cut1.data.begin());

        int sampleCutBegin2 = cutRange2.first*other.sampleRate;
        int sampleCutEnd2 = cutRange2.second*other.sampleRate;
        Audio cut2(other.sampleRate, other.bitCount, sampleCutEnd2-sampleCutBegin2);
        std::copy(other.data.begin()+sampleCutBegin2, other.data.begin()+sampleCutEnd2, cut2.data.begin());

        return cut1+cut2;
    }

    //Compute RMS: compute the RMS (per channel)
    float computeRMS() const{
        //1 channel MONO - default
        int iZero = 0;
        iZero = std::accumulate(data.begin(), data.end(), iZero,[](int x, T y){
                return x+y*y;
            }
        );
        float fResult = sqrt(iZero/(float)noSamples);
        return fResult;
    }

    //Sound normalization: normalize the sound files to the specified desired rms value (per channel)
    Audio normalizeSound(std::pair<float, float>desiredRMS) {
        float currentRMS = computeRMS();
        Audio result(sampleRate, bitCount, noSamples);
        //Normalize<T> normalize(desiredRMS.first, currentRMS, bitCount);
        /*if (bitCount==8){
            //Normalize<int8_t, 1> normalize(desiredRMS.first, currentRMS, bitCount);
            std::transform(data.begin(), data.end(), result.data.begin(), Normalize<int8_t>(desiredRMS.first, currentRMS, bitCount));
        } else if (bitCount==16) {
            //Normalize<int16_t, 1> normalize(desiredRMS.first, currentRMS, bitCount);
            std::transform(data.begin(), data.end(), result.data.begin(), Normalize<int16_t>(desiredRMS.first, currentRMS, bitCount));
        }*/
        std::transform(data.begin(), data.end(), result.data.begin(), Normalize<T> (desiredRMS.first, currentRMS, bitCount));
        return result;
    }

    //Fade-in: increase volume from 0 to 1.0
    Audio fadeIn(float seconds) const{
        int rampLength = seconds*sampleRate;
        int iZero = -1;
        Audio result(*this);
        std::for_each(result.data.begin(), result.data.begin()+rampLength, [rampLength, iZero](T& x)mutable{
                ++iZero;
                x = (iZero/(float)rampLength)*x;
                //return tResult;
            }
        );

        return result;
    }

    //Fade-out: decrease volume from 1.0 to 0
    Audio fadeOut(int seconds) const{
        int rampLength = seconds*sampleRate;
        int iZero = 0;
        Audio result(*this);
        std::for_each(result.data.begin(), result.data.begin()+rampLength, [rampLength, iZero](T& x)mutable{
                          ++iZero;
                          x = (1.0-iZero/(float)rampLength)*x;
                          //return tResult;
                      }
        );

        return result;
    }


    //load/save methods
    void load(std::string FileToOpen){
        std::ifstream ifsLoad(FileToOpen, std::ios::binary);

        if (!ifsLoad){
            //cerr << "Opening " << headerFilename << " failed :(" << endl;
            std::cout << "Opening " << FileToOpen << " failed :(" << std::endl;
            return;
        }

        //get filesize and resize vector
        ifsLoad.seekg (0, ifsLoad.end);
        std::streampos fileSize = ifsLoad.tellg();
        //data.resize(fileSize);

        //read in samples to vector
        noSamples = fileSize/ (sizeof(T)*channel);
        data.resize(noSamples);
        length = noSamples/(float)sampleRate;
        ifsLoad.seekg (0, ifsLoad.beg);
        ifsLoad.read((char*)&data[0], noSamples);

        ifsLoad.close();
    };
    void save(std::string FileToWriteTo){
        //Filename_samplingrate_samplesize_monoORstereo.raw
        std::string saveFilename = FileToWriteTo+'_'+std::to_string(sampleRate)+'_';
        if (bitCount==8)
            saveFilename += "8_";
        else if (bitCount==16)
            saveFilename += "16_";

        //if (channel == MONO)
        saveFilename += "mono.raw";
        //else
        //    saveFilename += "stereo.raw";

        std::ofstream ofsSave(saveFilename,std::ios::binary);

        ofsSave.write((char *)&data[0],data.size());
        ofsSave.flush();

        ofsSave.close();
    };

    /*//define begin()/end() to get iterator to start and "one-past" end
    iterator begin(void) const{
        return iterator(&data[0]);
    }

    iterator end(void) const{
        return iterator(&data[data.size()]);
    }*/



};

//STEREO - 2 channels

template <typename T>
class Audio<T, STEREO> {
private:
    std::vector<std::pair<T,T> > data;
    int sampleRate;
    int bitCount;
    int noSamples;
    float length;
public:
    //default constructor
    Audio(int sampleRateInHz=0):sampleRate(sampleRateInHz), noSamples(0), length(0){
        if (sizeof(T)==sizeof(int8_t))
            bitCount=8;
        else if (sizeof(T)==sizeof(int16_t))
            bitCount=16;
    }

    //parameterized constructor
    Audio(int SampleRate, int BitCount, int NoSamples):sampleRate(SampleRate), bitCount(BitCount), noSamples(NoSamples){
        data.resize(noSamples);
        length = noSamples/(float) sampleRate;
    }

    //test constructor
    Audio(int SampleRate, int BitCount, int NoSamples, std::initializer_list<unsigned char> lst):sampleRate(SampleRate), bitCount(BitCount), noSamples(NoSamples){
        data.resize(noSamples);
        auto onSample = lst.begin();
        for (int i=0;i<noSamples;++i){
            data[i].first = *onSample;
            ++onSample;
            data[i].second = *onSample;
            ++onSample;
        }
        length = noSamples/(float) sampleRate;
    }

    std::vector<std::pair<T,T>> getBuffer() {
        return data;
    }

    //destructor
    ~Audio(){
        data.clear();
    }

    //copy constructor
    Audio(const Audio& other):sampleRate(other.sampleRate), bitCount(other.bitCount), noSamples(other.noSamples), length(other.length), data(other.data){

    }

    //move constructor
    Audio(Audio&& other):sampleRate(other.sampleRate), bitCount(other.bitCount), noSamples(other.noSamples), length(other.length), data(std::move(other.data)){
        //zero out values
        other.sampleRate = 0;
        other.bitCount = 0;
        other.noSamples = 0;
        other.length = 0;
    }

    //copy assignment operator
    Audio& operator=(const Audio& rhs){
        //release
        std::vector<T,T> empty;
        data.swap(empty);

        //acquire and assign
        data = rhs.data;
        sampleRate = rhs.sampleRate;
        bitCount = rhs.bitCount;
        noSamples = rhs.noSamples;
        length = rhs.length;

        return *this;
    }

    //move assignment operator
    Audio& operator=(Audio&& rhs){
        //release
        std::vector<T,T> empty;
        data.swap(empty);

        //acquire resources
        data = std::move(rhs.data);
        data = rhs.data;
        sampleRate = rhs.sampleRate;
        bitCount = rhs.bitCount;
        noSamples = rhs.noSamples;
        length = rhs.length;

        //zero out values
        rhs.sampleRate = 0;
        rhs.bitCount = 0;
        rhs.noSamples = 0;
        rhs.length = 0;

        return *this;
    }

    //overloaded operations
    //A|B: concatenate audio file A and B
    Audio operator|(const Audio& rhs) const{
        Audio result(*this);
        result.data.resize(rhs.data.size()+data.size());
        for (int i = data.size(); i < result.data.size(); ++i) {
            result.data[i] = rhs.data[i-data.size()];
        }
        return result;
    }
    //A*F: volume factor A with F
    Audio operator*(const std::pair<float,float>& volumeFactors)const{
        Audio result(*this);
        for (int i = 0; i < result.data.size(); ++i) {
            result.data[i].first *= volumeFactors.first;
            result.data[i].second *= volumeFactors.second;
        }
        return result;
    }

    //A+B: add sound file amplitudes together (per sample)
    Audio operator+(const Audio& rhs) const{
        Audio result(*this);

        int max;
        if (bitCount==8)
            max = INT8_MAX;
        else if (bitCount==16)
            max = INT16_MAX;

        for (int i=0;i<data.size();++i){
            if (data[i].first+rhs.data[i].first>max)
                result.data[i].first = max;
            else
                result.data[i].first = data[i].first+rhs.data[i].first;
            if (data[i].second+rhs.data[i].second>max)
                result.data[i].second = max;
            else
                result.data[i].second = data[i].second+rhs.data[i].second;
        }

        return result;
    }

    //A^F: F will be a std::pair<int,int> specifies cut range to be cut from sound file A
    Audio operator^(const std::pair<int,int>& cutRange)const{
        Audio result(*this);

        int sampleCutBegin = cutRange.first*sampleRate;
        int sampleCutEnd = cutRange.second*sampleRate;

        result.data.erase(result.data.begin()+sampleCutBegin, result.data.begin()+sampleCutEnd);
        std::cout << "Size: " << result.data.size() << std::endl;

        return result;
    }

    //audio transformation
    //Reverse: reverse all samples
    Audio reverse() const {
        Audio result(*this);
        std::reverse(result.data.begin(),result.data.end());
        return result;
    }

    //Ranged add: select two (same length) sample ranges from two signals and add them together
    Audio rangedAdd(const std::pair<int,int>& cutRange1, const std::pair<int,int>& cutRange2, const Audio& other)const{
        //get sample ranges
        int sampleCutBegin1 = cutRange1.first*sampleRate;
        int sampleCutEnd1 = cutRange1.second*sampleRate;
        Audio cut1(sampleRate, bitCount, sampleCutEnd1-sampleCutBegin1);
        std::copy(data.begin()+sampleCutBegin1, data.begin()+sampleCutEnd1, cut1.data.begin());

        int sampleCutBegin2 = cutRange2.first*other.sampleRate;
        int sampleCutEnd2 = cutRange2.second*other.sampleRate;
        Audio cut2(other.sampleRate, other.bitCount, sampleCutEnd2-sampleCutBegin2);
        std::copy(other.data.begin()+sampleCutBegin2, other.data.begin()+sampleCutEnd2, cut2.data.begin());

        return cut1+cut2;
    }

    //Compute RMS: compute the RMS (per channel)
    std::pair<float,float> computeRMS() const{
        //2 channel - STEREO
        std::pair<int, int> pZero = std::make_pair(0,0);
        pZero = std::accumulate(data.begin(), data.end(), pZero,[](std::pair<int,int> x, std::pair<T,T> y){
                            x.first = x.first+(y.first*y.first);
                            x.second = x.second+(y.second*y.second);
                            return x;
                        }
        );
        float fLeftResult = sqrt(pZero.first/(float)noSamples);
        float fRightResult = sqrt(pZero.second/(float)noSamples);
        std::pair<float,float> pResult = std::make_pair(fLeftResult, fRightResult);
        return pResult;

    }

    //Sound normalization: normalize the sound files to the specified desired rms value (per channel)
    Audio normalizeSound(std::pair<float, float>desiredRMS) const{
        //float currentRMS = computeRMS();
        std::pair<float, float> currentRMS = computeRMS();
        Audio result(sampleRate, bitCount, noSamples);

        //Normalize<T, STEREO> normalize(desiredRMS, currentRMS, bitCount);
        if (bitCount==8){

            std::transform(data.begin(), data.end(), result.data.begin(), Normalize<int8_t , 2> (desiredRMS, currentRMS, bitCount));
        } else if (bitCount==16) {

            std::transform(data.begin(), data.end(), result.data.begin(), Normalize<int16_t , 2> (desiredRMS, currentRMS, bitCount));
        }

        return result;
    }

    //Fade-in: increase volume from 0 to 1.0
    Audio fadeIn(int seconds) const {
        int rampLength = seconds * sampleRate;
        int iZero = -1;
        Audio result(*this);
        std::for_each(result.data.begin(), result.data.begin() + rampLength, [rampLength, iZero](std::pair<T, T>& x)mutable {
                          ++iZero;
                          T tLeft = (iZero / (float) rampLength) * x.first;
                          T tRight = (iZero / (float) rampLength) * x.second;
                          x = std::make_pair(tLeft, tRight);
                          //return pResult;
                      }
        );

        return result;
    }

    //Fade-out: decrease volume from 1.0 to 0
    Audio fadeOut(int seconds) const{
        int rampLength = seconds*sampleRate;
        int iSample = noSamples-rampLength-1;
        Audio result(*this);
        std::for_each(result.data.end()-rampLength, result.data.end(), [rampLength, iSample](std::pair<T, T>& x)mutable{
                          ++iSample;
                          T tLeft = (1.0-iSample / (float) rampLength) * x.first;
                          T tRight = (1.0-iSample / (float) rampLength) * x.second;
                          x = std::make_pair(tLeft, tRight);
                          //return pResult;
                      }
        );

        return result;
    }


    //load/save methods
    void load(std::string FileToOpen){
        std::ifstream ifsLoad(FileToOpen, std::ios::binary);

        if (!ifsLoad){
            //cerr << "Opening " << headerFilename << " failed :(" << endl;
            std::cout << "Opening " << FileToOpen << " failed :(" << std::endl;
            return;
        }

        //get filesize and resize vector
        ifsLoad.seekg (0, ifsLoad.end);
        std::streampos fileSize = ifsLoad.tellg();
        //data.resize(fileSize);

        //read in samples to vector
        noSamples = fileSize/ (sizeof(T)*2);
        data.resize(noSamples);
        length = noSamples/(float)sampleRate;
        ifsLoad.seekg (0, ifsLoad.beg);

        for (int i=0;i<noSamples;++i){
            T leftSample;
            T rightSample;
            ifsLoad.read((char*)&leftSample, sizeof(T));
            ifsLoad.read((char*)&rightSample, sizeof(T));
            data[i] = std::make_pair(leftSample, rightSample);
        }

        ifsLoad.close();
    };
    void save(std::string FileToWriteTo){
        //Filename_samplingrate_samplesize_monoORstereo.raw
        std::string saveFilename = FileToWriteTo+'_'+std::to_string(sampleRate)+'_';
        if (bitCount==8)
            saveFilename += "8_";
        else if (bitCount==16)
            saveFilename += "16_";

        //if (channel == MONO)
        //    saveFilename += "mono.raw";
        //else
        saveFilename += "stereo.raw";

        std::ofstream ofsSave(saveFilename,std::ios::binary);

        //ofsSave.write(&data[0],data.size());
        for (int i=0;i<data.size();++i){
            ofsSave.write((char *)&data[i].first,sizeof(T));
            ofsSave.write((char *)&data[i].second,sizeof(T));
        }

        ofsSave.flush();

        ofsSave.close();
    };

    /*//define begin()/end() to get iterator to start and "one-past" end
    iterator begin(void) const{
        return iterator(&data[0]);
    }

    iterator end(void) const{
        return iterator(&data[data.size()]);
    }*/



};

#endif //ASSIGNMENT5_AUDIO_H
