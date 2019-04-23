#include "counter.h"
#include "math_simple.h"


inline void counter_add(u32 data, Counter * counter) {
    if (counter->cofb == OFB_SATURATE) {
        counter->data = saturate_add(counter->data, data, counter->data_max);
    } else {
        //Native number type overflow, needs special care
        if (counter->data + data < counter->data) {
            u32 temp = counter->data_max - counter->data;
            u32 data_rem = data % counter->data_max;
            if (temp > data_rem) {
                //This should never happen
                counter->data = (data % (counter->data_max * 2)) - temp;
            } else {
                counter->data = data_rem - temp;
            }
        } else {
            counter->data = (counter->data + data) % counter->data_max;
        }
    }
}

inline void counter_sub(u32 data, Counter * counter) {
    if (counter->cofb == OFB_SATURATE) {
        counter->data = saturate_sub(counter->data, data);
    } else {
        if (counter->data - data > counter->data) {
            u32 temp = counter->data_max - (data % counter->data_max);
            //Add 1, because if it's currently 49 with a max of 100, and we subtract 50, the correct result is 100, not 99;
            counter_add(temp + 1, counter);
        } else {
            counter->data -= data;
        }
    }
}

inline void counter_inc(Counter * counter) {
    if (counter->cofb == OFB_SATURATE) {
        counter->data = saturate_inc(counter->data, counter->data_max);
    } else {

        if (counter->data == counter->data_max) {
            counter->data = 0;
        } else {
            counter->data += 1;
        }
    }
}

inline void counter_dec(Counter * counter) {
    if (counter->cofb == OFB_SATURATE) {
        counter->data = saturate_dec(counter->data);
    } else {
        if (!counter->data) {
            counter->data = counter->data_max;
        } else {
            counter->data -= 1;
        }
    }
}

inline void counter_set(u32 data, Counter * counter) {
    if (data > counter->data_max) {
        counter->data = counter->data_max;
    } else {
        counter->data = data;
    }
}
