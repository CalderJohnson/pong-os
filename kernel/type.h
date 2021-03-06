/* useful semantic constants */

#ifndef TYPE_H
#define TYPE_H

#define bool               _Bool
#define true               1
#define false              0

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef char               int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned int       size_t;

#define NULL               ((void*)0)
#define UNUSED(x)          (void)(x)

#endif