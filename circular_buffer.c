#include "circular_buffer.h"

/**
 * Reset buffer to a no pending data state
 * All pending data will zeroed out
 */
void reset(Circular_buffer* buffer) {
    Buffer_read_result result;
    do {
        pop_from_buffer(buffer, &result);
        //Do it until we have something to read.
    } while (result.readSuccess);
}
