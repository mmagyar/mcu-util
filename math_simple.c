#include "math_simple.h"

inline u32 saturate_add(u32 data1, u32 data2, u32 max) {
    u32 added = data1 + data2;
    //If the added value is above maximum value or would overflow, return max
    if (added > max || added < data1) return max;
    else return added;
}

inline u32 saturate_sub(u32 data1, u32 data2) {
    u32 subbed = data1 - data2;
    if (subbed > data1) {
        return 0;
    } else {
        return subbed;
    }
}

inline u32 saturate_mul(u32 data1, u32 data2, u32 max) {
    u64 result = ((u64) data1) * ((u64) data2);
    if (result > max) return max;
    return (u32) result;

}

inline u32 saturate_inc(u32 data, u32 max) {
    if (data < max) return data + 1;
    return max;
}

inline u32 saturate_dec(u32 data) {
    if (data) return data - 1;
    return 0;
}

u32 saturate_set(u32 data, u32 max) {
    if (data > max) return max;
    return data;
}
