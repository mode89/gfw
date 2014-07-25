#ifndef __COMMON_CRC32_H__
#define __COMMON_CRC32_H__

#include "cmn/typedefs.h"

namespace Common {

    uint32_t CRC32( uint32_t crc, const void * data, uint32_t size );
    uint32_t CRC32( const char * string );

} // namespace Common

#endif // __COMMON_CRC32_H__
