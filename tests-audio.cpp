#include "Audio.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("OVERLOADED OPERATORS","[OverloadedOperators]"){
    Audio<int8_t> a1(2,8, 4, {1,2,3,4} );
    Audio<int8_t> a2{2,8,6,{1,2,3,4,5,6}};
    Audio<int16_t, STEREO> a3{2,16,6,{1,2,2,3,3,4,4,5,5,6,6,7}};
    Audio<int16_t, STEREO> a4{2,16,4,{1,2,2,3,3,4,4,5}};
    Audio<int8_t> a5(2,8, 2, {120,80} );
    Audio<int16_t, STEREO> a6{2,16,2,{120,80,50,40}};
    Audio<int8_t> a7(2,8, 4, {50,126,4,51} );
    Audio<int16_t, STEREO> a8{2,16,2,{80,120,50,40}};

    SECTION("CONCATENATE MONO"){
        Audio<int8_t> aOut = a1|a2;
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==1);
        REQUIRE(buf[1]==2);
        REQUIRE(buf[2]==3);
        REQUIRE(buf[3]==4);
        REQUIRE(buf[4]==1);
        REQUIRE(buf[5]==2);
        REQUIRE(buf[6]==3);
        REQUIRE(buf[7]==4);
        REQUIRE(buf[8]==5);
        REQUIRE(buf[9]==6);
    }

    SECTION("CONCATENATE STEREO"){
        Audio<int16_t, STEREO> aOut = a3|a4;
        auto buf = aOut.getBuffer();
        //cout << buf[0].first << endl;
        REQUIRE(buf[0].first==1);
        REQUIRE(buf[0].second==2);
        REQUIRE(buf[1].first==2);
        REQUIRE(buf[1].second==3);
        REQUIRE(buf[2].first==3);
        REQUIRE(buf[2].second==4);
        REQUIRE(buf[3].first==4);
        REQUIRE(buf[3].second==5);
        REQUIRE(buf[4].first==5);
        REQUIRE(buf[4].second==6);
        REQUIRE(buf[5].first==6);
        REQUIRE(buf[5].second==7);
        REQUIRE(buf[6].first==1);
        REQUIRE(buf[6].second==2);
        REQUIRE(buf[7].first==2);
        REQUIRE(buf[7].second==3);
        REQUIRE(buf[8].first==3);
        REQUIRE(buf[8].second==4);
        REQUIRE(buf[9].first==4);
        REQUIRE(buf[9].second==5);
    }

    SECTION("VOLUME FACTOR MONO"){
        pair<float, float> F = make_pair(0.8f,0.5f);
        Audio<int8_t> aOut = a5*F;
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==96);
        REQUIRE(buf[1]==64);
    }

    SECTION("VOLUME FACTOR STEREO"){
        pair<float, float> F = make_pair(0.8f,0.5f);
        Audio<int16_t, STEREO> aOut = a6*F;
        auto buf = aOut.getBuffer();//120,80,50,40
        REQUIRE(buf[0].first==96);
        REQUIRE(buf[0].second==40);
        REQUIRE(buf[1].first==40);
        REQUIRE(buf[1].second==20);
    }

    SECTION("ADD MONO"){
        Audio<int8_t> aOut = a1+a7;
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==51);
        REQUIRE(buf[1]==127);
        REQUIRE(buf[2]==7);
        REQUIRE(buf[3]==55);
    }

    SECTION("ADD STEREO"){
        Audio<int16_t, STEREO> aOut = a6+a8;
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0].first==200);
        REQUIRE(buf[0].second==200);
        REQUIRE(buf[1].first==100);
        REQUIRE(buf[1].second==80);
    }

    SECTION("CUT MONO"){
        pair<int,int> cutRange = make_pair(0,2);
        Audio<int8_t> aOut = a2^cutRange;
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==5);
        REQUIRE(buf[1]==6);
    }

    SECTION("CUT STEREO"){
        pair<int,int> cutRange = make_pair(1,2);
        Audio<int16_t, STEREO> aOut = a3^cutRange;
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0].first==1);
        REQUIRE(buf[0].second==2);
        REQUIRE(buf[1].first==2);
        REQUIRE(buf[1].second==3);
        REQUIRE(buf[2].first==5);
        REQUIRE(buf[2].second==6);
        REQUIRE(buf[3].first==6);
        REQUIRE(buf[3].second==7);
    }

}


TEST_CASE("AUDIO TRANSFORMATION OPERATORS","[AudioTransformationOperators]"){
    Audio<int16_t> a1(2,16, 4, {1,2,3,4} );
    Audio<int16_t> a2{2,16,6,{1,2,3,4,5,6}};
    Audio<int8_t, STEREO> a3{2,8,6,{1,2,2,3,3,4,4,5,5,6,6,7}};
    Audio<int8_t, STEREO> a4{2,8,4,{1,2,2,3,3,4,4,5}};
    Audio<int16_t, STEREO> a5{2,16,2,{120,80,50,40}};

    SECTION("REVERSE MONO"){
        Audio<int16_t> aOut = a2.reverse();
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==6);
        REQUIRE(buf[1]==5);
        REQUIRE(buf[2]==4);
        REQUIRE(buf[3]==3);
        REQUIRE(buf[4]==2);
        REQUIRE(buf[5]==1);
    }

    SECTION("REVERSE STEREO"){
        Audio<int8_t, STEREO> aOut = a3.reverse();
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0].first==6);
        REQUIRE(buf[0].second==7);
        REQUIRE(buf[1].first==5);
        REQUIRE(buf[1].second==6);
        REQUIRE(buf[2].first==4);
        REQUIRE(buf[2].second==5);
        REQUIRE(buf[3].first==3);
        REQUIRE(buf[3].second==4);
        REQUIRE(buf[4].first==2);
        REQUIRE(buf[4].second==3);
        REQUIRE(buf[5].first==1);
        REQUIRE(buf[5].second==2);
    }

    SECTION("RANGED ADD MONO"){
        pair<int, int> cutRange1 = make_pair(0,2);
        pair<int, int> cutRange2 = make_pair(1,3);
        Audio<int16_t> aOut = a1.rangedAdd(cutRange1, cutRange2, a2);
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==4);
        REQUIRE(buf[1]==6);
        REQUIRE(buf[2]==8);
        REQUIRE(buf[3]==10);
    }

    SECTION("RANGED ADD STEREO"){
        pair<int, int> cutRange1 = make_pair(0,1);
        pair<int, int> cutRange2 = make_pair(1,2);
        Audio<int8_t, STEREO> aOut = a3.rangedAdd(cutRange1, cutRange2, a4);
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0].first==4);
        REQUIRE(buf[0].second==6);
        REQUIRE(buf[1].first==6);
        REQUIRE(buf[1].second==8);
    }

    SECTION("COMPUTE RMS MONO"){
        float fRMS = a1.computeRMS();
        REQUIRE(fRMS==2.738612788f);
    }

    SECTION("COMPUTE RMS STEREO"){
        pair<float,float> fRMS = a4.computeRMS();
        REQUIRE(fRMS.first==2.738612788f);
        REQUIRE(fRMS.second==3.674234614f);
    }

    SECTION("SOUND NORMALIZATION MONO"){
        pair<float,float> RMSdesired = make_pair(5.0f,2.8f);
        Audio<int16_t> aOut = a1.normalizeSound(RMSdesired);
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==1);
        REQUIRE(buf[1]==3);
        REQUIRE(buf[2]==5);
        REQUIRE(buf[3]==7);
    }

    SECTION("SOUND NORMALIZATION STEREO"){
        pair<float,float> RMSdesired = make_pair(33.4f,20.0f);
        Audio<int16_t, STEREO> aOut = a5.normalizeSound(RMSdesired);
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0].first==43);
        REQUIRE(buf[0].second==25);
        REQUIRE(buf[1].first==18);
        REQUIRE(buf[1].second==12);
    }


}

TEST_CASE("EXTRA CREDIT","[ExtraCredit]"){
    Audio<int16_t> a1{2,16,10,{40,46,58,61,24,20,48,77,15,90}};
    Audio<int8_t, STEREO> a2{2,8,6,{24,51,63,48,52,67,98,94,43,110,64,66}};

    SECTION("FADE IN MONO"){
        Audio<int16_t> aOut = a1.fadeIn(3);
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0]==0);
        REQUIRE(buf[1]==7);
        REQUIRE(buf[2]==19);
        REQUIRE(buf[3]==30);
        REQUIRE(buf[4]==16);
        REQUIRE(buf[5]==16);
    }

    SECTION("FADE IN STEREO"){
        Audio<int8_t, STEREO> aOut = a2.fadeIn(1);
        auto buf = aOut.getBuffer();
        REQUIRE(buf[0].first==0);
        REQUIRE(buf[0].second==0);
        REQUIRE(buf[1].first==31);
        REQUIRE(buf[1].second==24);
    }

    /*SECTION("FADE OUT MONO"){
      Audio<int16_t> aOut = a1.fadeOut(3);
      auto buf = aOut.getBuffer();
      REQUIRE(buf[0]==0);
      REQUIRE(buf[1]==7);
      REQUIRE(buf[2]==19);
      REQUIRE(buf[3]==30);
      REQUIRE(buf[4]==16);
      REQUIRE(buf[5]==16);
    }

    SECTION("FADE OUT STEREO"){
      Audio<int8_t, STEREO> aOut = a2.fadeIn(1);
      auto buf = aOut.getBuffer();
      REQUIRE(buf[0].first==0);
      REQUIRE(buf[0].second==0);
      REQUIRE(buf[1].first==31);
      REQUIRE(buf[1].second==24);
    }*/
}
