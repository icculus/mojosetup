#ifndef _INCL_UNIVERSAL_H_
#define _INCL_UNIVERSAL_H_

/* Include this file from everywhere...it provides basic type sanity, etc. */

#ifdef __cplusplus
extern "C" {
#endif

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef int boolean;

#ifdef __cplusplus
}
#endif

#endif

/* end of universal.h ... */

