#ifndef ASSIGNMENT5_NORMALIZE_H
#define ASSIGNMENT5_NORMALIZE_H

#include "Audio.h"
#include <cstdint>

template <typename T, int channel=1 >
class Normalize{
public:
  float RMSdesired;
  float RMScurrent;
  int bitCount;
    Normalize(float rmsDes, float rmsCurr, int iBitCount):RMSdesired(rmsDes), RMScurrent(rmsCurr),bitCount(iBitCount){

    }

    float operator()(const T& value)const{
        float fResult = value*RMSdesired/RMScurrent;
        if (bitCount==8){
            if (fResult>INT8_MAX)
                fResult = INT8_MAX;
            if (fResult<INT8_MIN)
                fResult = INT8_MIN;
        } else if (bitCount == 16){
            if (fResult>INT16_MAX)
                fResult = INT16_MAX;
            if (fResult<INT16_MIN)
                fResult = INT16_MIN;
        }
        return fResult;
    }


};

template <typename T>
class Normalize<T, 2>{
public:
  std::pair<float, float> RMSdesired;
  std::pair<float, float> RMScurrent;
  int bitCount;
    Normalize(std::pair<float, float> rmsDes, std::pair<float, float> rmsCurr, int iBitCount):RMSdesired(rmsDes), RMScurrent(rmsCurr),bitCount(iBitCount){

    }

    std::pair<float, float> operator()(const std::pair<T,T>& value)const{
        float fLeft = value.first*RMSdesired.first/RMScurrent.first;
        if (bitCount==8){
            if (fLeft>INT8_MAX)
                fLeft = INT8_MAX;
            if (fLeft<INT8_MIN)
                fLeft = INT8_MIN;
        } else if (bitCount == 16){
            if (fLeft>INT16_MAX)
                fLeft = INT16_MAX;
            if (fLeft<INT16_MIN)
                fLeft = INT16_MIN;
        }

        float fRight = value.second*RMSdesired.second/RMScurrent.second;
        if (bitCount==8){
            if (fRight>INT8_MAX)
                fRight = INT8_MAX;
            if (fRight<INT8_MIN)
                fRight = INT8_MIN;
        } else if (bitCount == 16){
            if (fRight>INT16_MAX)
                fRight = INT16_MAX;
            if (fRight<INT16_MIN)
                fRight = INT16_MIN;
        }
        std::pair<float ,float> pResult = std::make_pair(fLeft, fRight);
        return pResult;
    }


};


#endif //ASSIGNMENT5_NORMALIZE_H
