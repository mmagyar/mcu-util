#ifndef UTIL_FIXED_POINT_H_
#define UTIL_FIXED_POINT_H_

#include "types.h"

#define NEGATIVE_LEFT_SHIFT true
#define fraction 16
typedef  s16 sfp8_8;

typedef  s32 sfixed;
typedef  u32 ufixed;

sfixed  f_div(sfixed x, sfixed y) {
    return ((s64)x * (1 << fraction)) / y;
}

sfixed  f_mul(sfixed x, sfixed y) {
    #if NEGATIVE_LEFT_SHIFT
        return ((s64)x * (s64)y) >> fraction;
    #else
        return ((s64)x * (s64)y) / (1 << fraction);
    #endif // NEGATIVE_LEFT_SHIFT


}

typedef struct {
    s32 main : 8;
    s32 frac : 24;
}S824;

#endif
