#include "circular_buffer.h"

CIRCULAR_BUFFER_INDEX_TYPE advance_cursor(
CIRCULAR_BUFFER_INDEX_TYPE current, CIRCULAR_BUFFER_INDEX_TYPE size);

inline CIRCULAR_BUFFER_INDEX_TYPE advance_cursor(
CIRCULAR_BUFFER_INDEX_TYPE current, CIRCULAR_BUFFER_INDEX_TYPE size) {
    return current + 1 >= size ? 0 : current + 1;
}

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

/**
 * Adds a value to the buffer in a circular fashion
 * If the buffer is full, log an error and overwrite the last value
 */
inline void add_to_buffer(Circular_buffer *buffer, CIRCULAR_BUFFER_DATA_TYPE data) {
    CIRCULAR_BUFFER_INDEX_TYPE newPosition = advance_cursor(
            buffer->writePosition, buffer->size);

    //Catching up to the read pointer means that the buffer has overflow
    if (newPosition == buffer->readPosition) {
        log_error(EC_CIRCULAR_BUFFER_OVERFLOW, buffer->identifier);
        //On error, just push the read position, overriding old data
        buffer->readPosition = advance_cursor(buffer->readPosition,
                buffer->size);
    }

    buffer->buffer[buffer->writePosition] = data;
    buffer->writePosition = newPosition;
}

/**
 * Add value to the buffer, if there are free space.
 * returns true on success, false if there is no space
 */
inline bool add_to_buffer_if_not_full(Circular_buffer *buffer, char data) {
    CIRCULAR_BUFFER_INDEX_TYPE newPosition = advance_cursor(
            buffer->writePosition, buffer->size);

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
 * result is the output value, leaves the current value in the buffer
 */
inline void get_from_buffer(Circular_buffer *buffer, Buffer_read_result* result) {
    CIRCULAR_BUFFER_INDEX_TYPE index = advance_cursor(buffer->readPosition,
            buffer->size);

    //This means that there are pending bytes
    result->readSuccess = index != buffer->writePosition;
    if (result->readSuccess) {
        result->data = buffer->buffer[index];
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
    CIRCULAR_BUFFER_INDEX_TYPE index = advance_cursor(buffer->readPosition,
            buffer->size);

    //This means that there are pending bytes
    result->readSuccess = index != buffer->writePosition;

    if (result->readSuccess) {
        result->data = buffer->buffer[index];
        buffer->buffer[index] = 0;
        buffer->readPosition = index;
    }
}
