#ifndef UTIL_CIRCULAR_BUFFER_H_
#define UTIL_CIRCULAR_BUFFER_H_
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "system_defs.h"

#ifndef CIRCULAR_BUFFER_INDEX_TYPE
#define CIRCULAR_BUFFER_INDEX_TYPE uint16_t
#endif

#ifndef CIRCULAR_BUFFER_DATA_TYPE
#define CIRCULAR_BUFFER_DATA_TYPE uint8_t
#endif

#define def_Circular_buffer(name, size, identifier)    \
    CIRCULAR_BUFFER_DATA_TYPE name ## _buffer[(1 << size)]; \
    Circular_buffer name = {identifier, size, 0 , (1 << size) - 1 , name ## _buffer}

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
    uint8_t size;
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

inline CIRCULAR_BUFFER_INDEX_TYPE advance_cursor(CIRCULAR_BUFFER_INDEX_TYPE current, CIRCULAR_BUFFER_INDEX_TYPE size) {
    return (((current) + 1) & ((1 << size) - 1));
}

/**
 * Adds a value to the buffer in a circular fashion
 * If the buffer is full, log an error and overwrite the last value
 */
inline void add_to_buffer(Circular_buffer *buffer, CIRCULAR_BUFFER_DATA_TYPE data) {
	CIRCULAR_BUFFER_INDEX_TYPE newPosition = advance_cursor(buffer->writePosition, buffer->size);

    //Catching up to the read pointer means that the buffer has overflow
    if (newPosition == buffer->readPosition) {
        log_error(EC_CIRCULAR_BUFFER_OVERFLOW, buffer->identifier);
        //On error, just push the read position, overriding old data
        buffer->readPosition = advance_cursor(buffer->readPosition, buffer->size);
    }

    buffer->buffer[buffer->writePosition] = data;
    buffer->writePosition = newPosition;
}

/**
 * Add value to the buffer, if there are free space.
 * returns true on success, false if there is no space
 */
inline bool add_to_buffer_if_not_full(Circular_buffer *buffer, char data) {
	CIRCULAR_BUFFER_INDEX_TYPE newPosition = advance_cursor(buffer->writePosition, buffer->size);

    //Catching up to the read pointer means that the buffer has overflow
    if (newPosition == buffer->readPosition) {
        return false;
    }

    buffer->buffer[buffer->writePosition] = data;
    buffer->writePosition = newPosition;
    return true;
}

/**
 * gets a value from the buffer in a circular fashion, advances the position of the cursor
 result is the output value, leaves the current value in the buffer
 *
 */
inline void get_from_buffer(Circular_buffer *buffer, Buffer_read_result* result) {
    CIRCULAR_BUFFER_INDEX_TYPE index =  advance_cursor(buffer->readPosition, buffer->size);
    result->data = buffer->buffer[index];
    //This means that there are pending bytes
    result->readSuccess = index != buffer->writePosition;
    if (result->readSuccess) {
        buffer->readPosition = index;
    }
}

/**
 * gets a value from the buffer in a circular fashion, advances the position of the cursor
 result is the output value
 Resets the value in the buffer to 0
 *
 */
inline void pop_from_buffer(Circular_buffer *buffer, Buffer_read_result* result) {
    CIRCULAR_BUFFER_INDEX_TYPE index =  advance_cursor(buffer->readPosition, buffer->size);

    result->data = buffer->buffer[index];
    //This means that there are pending bytes
    result->readSuccess = index != buffer->writePosition;

    buffer->buffer[index] = 0;
    if (result->readSuccess) {
        buffer->readPosition = index;
    }
}

/**
 * Reset buffer to a no pending data state
 * All pending data will zeroed out
 */
void reset(Circular_buffer* buffer);

#endif /* UTIL_CIRCULAR_BUFFER_H_ */
