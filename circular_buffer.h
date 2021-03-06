#ifndef UTIL_CIRCULAR_BUFFER_H_
#define UTIL_CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef DONT_USE_ERROR_LOG
#define log_error(a, b)
#define EC_CIRCULAR_BUFFER_OVERFLOW
#else
#include "error_log.h"
#endif

#ifndef CIRCULAR_BUFFER_INDEX_TYPE
#define CIRCULAR_BUFFER_INDEX_TYPE uint16_t
#endif

#ifndef CIRCULAR_BUFFER_DATA_TYPE
#define CIRCULAR_BUFFER_DATA_TYPE uint8_t
#endif

/**
 * Size in number of values
 */
#define def_Circular_buffer(name, size, identifier)    \
    CIRCULAR_BUFFER_DATA_TYPE name ## _buffer[(size)]; \
    Circular_buffer name = {identifier, size, 0 , (size) - 1 , name ## _buffer}

/**
 * identifier - helps debugging,so we know what buffer we are working with
 * size - the size of the buffer in  powers of 2: 1 = 2, 2 = 4, 3 = 8, 4 = 16,
 *  5 = 32,6 = 64, 7 = 128, 8 = 256, etc...
 * writePosition -  cursor position for writing data,
 *                  the position stored here is where we update the value last time
 *                   If this value reaches the read cursor an error will be logged,
 *                   and the read cursor being pushed, overwriting the old data
 * readPosition  -  cursor position for reading the data, reading the data advances this cursor.
 *                  If this value reaches the write cursor, the return value will indicate that
 *                  we have read all the data from the buffer
 * buffer        - The backing data store location
 */
typedef struct {
    char identifier;
    uint16_t size;
    CIRCULAR_BUFFER_INDEX_TYPE writePosition;
    CIRCULAR_BUFFER_INDEX_TYPE readPosition;
    CIRCULAR_BUFFER_DATA_TYPE *buffer;
} Circular_buffer;

/**
 * Used as a return value for reading data from the buffer,
 * contains the data itself, and an indicator that there are more data in the buffer
 */
typedef struct {
    CIRCULAR_BUFFER_DATA_TYPE data;
    bool readSuccess;
} Buffer_read_result;

/**
 * Adds a value to the buffer in a circular fashion
 * If the buffer is full, log an error and overwrite the last value
 */
void add_to_buffer(Circular_buffer *buffer, CIRCULAR_BUFFER_DATA_TYPE data);

/**
 * Add value to the buffer, if there are free space.
 * returns true on success, false if there is no space
 */
bool add_to_buffer_if_not_full(Circular_buffer *buffer, char data);

/**
 * gets a value from the buffer in a circular fashion, advances the position of the cursor
 * result is the output value, leaves the current value in the buffer
 */
void get_from_buffer(Circular_buffer *buffer, Buffer_read_result* result);

/**
 * gets a value from the buffer in a circular fashion, advances the position of the cursor
 result is the output value
 Resets the value in the buffer to 0
 *
 */
void pop_from_buffer(Circular_buffer *buffer, Buffer_read_result* result);
/**
 * Reset buffer to a no pending data state
 * All pending data will zeroed out
 */
void reset(Circular_buffer* buffer);

#endif /* UTIL_CIRCULAR_BUFFER_H_ */
