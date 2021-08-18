#include "MyMediaInfoKey.h"
#if 0
bool operator < (const MyMediaInfoKey & key1, const MyMediaInfoKey & key2)
{
    #if 0
    return (key1.mediaTypeId() < key2.mediaTypeId() || key1.streamDirection() < key2.streamDirection());
    #else
    return (key1.mediaTypeId() < key2.mediaTypeId()) || 
       ( !( key2.mediaTypeId() < key1.mediaTypeId()) && (key1.streamDirection() < key2.streamDirection() ) );
    #endif
}
#endif
