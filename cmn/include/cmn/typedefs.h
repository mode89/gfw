#ifndef __COMMON_TYPEDEFS_H__
#define __COMMON_TYPEDEFS_H__

typedef char                    char8_t;
typedef signed char             int8_t;
typedef unsigned char           uint8_t;
typedef signed short            int16_t;
typedef unsigned short          uint16_t;
typedef int                     int32_t;
typedef unsigned int            uint32_t;
typedef long long int           int64_t;
typedef unsigned long long int  uint64_t;
typedef unsigned char           bool8_t;

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#ifndef NULL
#define NULL 0
#endif // NULL

#endif // __COMMON_TYPEDEFS_H__
