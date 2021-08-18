#include <string>
#include <iostream> 
#include <fstream> 
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "MyMediaInfoKey.h"
//using namespace std; 
#define DEBUG_FUHU(format,...) {printf("FUHU>>Line: %d ", __LINE__);printf(format, ##__VA_ARGS__);printf("\n");fflush(stdout);}
enum MediaTypeIDType
    {
        MEDIA_TYPE_ID_NOT_APPLICABLE    = 9997,
        MEDIA_TYPE_ID_VOICE             = 0,
        MEDIA_TYPE_ID_VIDEO             = 1,
        MEDIA_TYPE_ID_DATA              = 2,
        MEDIA_TYPE_ID_CONTROL           = 3,
        MEDIA_TYPE_ID_APPLICATION       = 4,
        MEDIA_TYPE_ID_OTHER             = 5,
        MEDIA_TYPE_ID_TEXT              = 6,
        MEDIA_TYPE_ID_NONE              = 9996,
        MEDIA_TYPE_ID_UNKNOWN           = 9999
    };
enum RtpStreamDirection
        
{
    FORWARD_DIR = 0,
    REVERSE_DIR = 1,
    UNKNOWN_DIR = 2
};

typedef std::map<MyMediaInfoKey,int> VoiceRtpDrMap;

void printRtpMap(VoiceRtpDrMap& rtpMap)
{
    int index = 0;
    for (VoiceRtpDrMap::iterator it = rtpMap.begin(); it != rtpMap.end(); ++it)
    {
        const MyMediaInfoKey *segmentKey = &(it->first);
        //VoiceMedDrBase * pMedDrBase = it->second ;
        DEBUG_FUHU("map key index: %d ( mediaTypeId:%d, streamDirection:%d )",
            index, (int)segmentKey->mediaTypeId(), (int)segmentKey->streamDirection());
        index++;
    }
}

void wrapper_find(VoiceRtpDrMap& map, short int mediaType, char dir)
{
     MyMediaInfoKey key(mediaType, dir);
     VoiceRtpDrMap::iterator itr = map.find(key);
    if (itr == map.end())
    {
        DEBUG_FUHU("look up: mediaType:%d, direction:%d: return null", 
            (int)mediaType, (int)dir);
    }
      else 
    {
        DEBUG_FUHU("look up: mediaType:%d, direction:%d: return ok", 
            (int)mediaType, (int)dir);
    }
}

int main(int argc, char* argv[])
{
    VoiceRtpDrMap rtpMapforFake;
    #if 0
    //issue1
    MyMediaInfoKey key_ref1(1,0); //0,1
    MyMediaInfoKey key_ref2(0,1); //1,0
    #else
    //issue2
    MyMediaInfoKey key_ref1(0,1); //0,1
    MyMediaInfoKey key_ref2(1,0); //1,0
    #endif
    
    #if 1
    MyMediaInfoKey key_ref3(1,1); //1,1
    MyMediaInfoKey key_ref4(0,0); //0,0
    #endif
    
    #if 0
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref4,1));
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref2,2));

    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref1,1));
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref3,2));
    #else
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref1,1));
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref2,2));
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref3,1));
    rtpMapforFake.insert(VoiceRtpDrMap::value_type(key_ref4,2));
    #endif
    
    printRtpMap(rtpMapforFake);
    DEBUG_FUHU("test begin");
    //exit(0);
    int tmpMediaType = 0;
    for (tmpMediaType = 0; tmpMediaType < 2; tmpMediaType++)
    {
    DEBUG_FUHU("\n############## %d look up begin ####################", tmpMediaType);
    wrapper_find(rtpMapforFake, tmpMediaType, FORWARD_DIR);
    wrapper_find(rtpMapforFake, tmpMediaType, REVERSE_DIR);
    #if 0
    DEBUG_FUHU("tmpMediaType:%d, itr1== null(%s), itr2== null(%s)", 
        (int)tmpMediaType, 
        (rtpMapforFake.end() == itr1) ? "true": "false",
        (rtpMapforFake.end() == itr2)? "true": "false");


    DEBUG_FUHU("key1 < key_ref1: %s, key_ref1 < key1:%s", 
                key1 < key_ref1 ? "ture" : "false",
                key_ref1 < key1 ? "ture" : "false");
     DEBUG_FUHU("key1 < key_ref2: %s, key_ref2 < key1:%s", 
        key1 < key_ref2 ? "ture" : "false",
        key_ref2 < key1 ? "ture" : "false");

    DEBUG_FUHU("key2 < key_ref1: %s, key_ref1 < key2:%s", 
        key2 < key_ref1 ? "ture": "false",
        key_ref1 < key2 ? "ture" : "false");

    DEBUG_FUHU("key2 < key_ref2: %s, key_ref2 < key2:%s", 
        key2 < key_ref2 ? "ture" : "false",
        key_ref2 < key2 ? "ture" : "false");
    #endif
    DEBUG_FUHU("############## %d look up end ####################\n", tmpMediaType);
    }
    DEBUG_FUHU("test end");
}