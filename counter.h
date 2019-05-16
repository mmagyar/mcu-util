#ifndef MCU_UTIL_COUNTER_H_
#define MCU_UTIL_COUNTER_H_

#include "types.h"

typedef enum Counter_overflow_behaviour {
    OFB_SATURATE = 0,
    OFB_WRAP_AROUND = 1
} Counter_overflow_behaviour;

#define COUNTER_SATURATE(MAX) {MAX, OFB_SATURATE, 0}
#define COUNTER_WRAP_AROUND(MAX) {MAX, OFB_WRAP_AROUND, 0}
typedef struct Counter {
    u32 data_max;
    Counter_overflow_behaviour cofb;
    u32 data;
} Counter;


void counter_inc(Counter * counter);
void counter_dec(Counter * counter);
void counter_add(u32 data, Counter * counter);
void counter_set(u32 data, Counter * counter);

void counter_sub(u32 data, Counter * counter);

#endif /* MCU_UTIL_COUNTER_H_ */
