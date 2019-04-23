#ifndef UTIL_MATH_SIMPLE_H_
#define UTIL_MATH_SIMPLE_H_
#include "types.h"

#define abs(x) (((x) >= 0) ? (x) : -(x));

u32 saturate_add(u32 data1, u32 data2, u32 max);
u32 saturate_sub(u32 data1, u32 data2);
u32 saturate_mul(u32 data1, u32 data2, u32 max);
u32 saturate_inc(u32 data, u32 max);
u32 saturate_dec(u32 data);
u32 saturate_set(u32 data, u32 max);

#endif /* UTIL_MATH_SIMPLE_H_ */
