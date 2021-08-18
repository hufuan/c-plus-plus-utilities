#ifndef MyMediaInfoKey__H
#define MyMediaInfoKey__H
#include <stdio.h>
typedef unsigned short UShort;
class MyMediaInfoKey
{
public:
    MyMediaInfoKey(UShort media, char direction):
        mediaTypeId_(media),
        streamDirection_(direction)
        {

        }

    MyMediaInfoKey(const MyMediaInfoKey & key)
    {
            mediaTypeId_ = key.mediaTypeId_;
            streamDirection_ = key.streamDirection_;
    }

    MyMediaInfoKey & operator = (const MyMediaInfoKey & key)
    {
        if(this != &key)
        {
            mediaTypeId_ = key.mediaTypeId_;
            streamDirection_ = key.streamDirection_;
        }

        return *this;
    }
    
    inline bool operator < (const MyMediaInfoKey & key) const
    {
    #if 0
        return ((mediaTypeId_ < key.mediaTypeId_) || 
        ( !(key.mediaTypeId_ < mediaTypeId_) && (streamDirection_ < key.streamDirection_)) );
    #else
        return ((mediaTypeId_ < key.mediaTypeId_) || (streamDirection_ < key.streamDirection_));
    #endif
    }
    
    UShort mediaTypeId() const {return mediaTypeId_;}
    void mediaTypeId(UShort val) { mediaTypeId_ = val;}

    char streamDirection() const {return streamDirection_;}
    void streamDirection(char val) { streamDirection_ = val;}
private:
    UShort mediaTypeId_;
    char streamDirection_;
};
#endif