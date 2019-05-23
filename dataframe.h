#ifndef UTIL_DATAFRAME_H_
#define UTIL_DATAFRAME_H_
#include "types.h"
#include "stdbool.h"

#define FRAME_START 0xFA
#define FRAME_END_CRC32 0xFB
#define FRAME_END 0xFC
#define FRAME_ESC 0XFD

#ifndef FRAME_BUFFER_SIZE
#define FRAME_BUFFER_SIZE 128
#endif

typedef struct {
    bool received_start;
    bool received_end;
    bool received_esc;
    u8 buffer[FRAME_BUFFER_SIZE];
    u8 buffer_size;
    u8 message_size;
    u32 crc;
    u32 crc_counter;
    u16 buffer_cursor;
} Frame_Receive_buffer;

/**
 * Returns true if transmission is finished
 */
bool receive_byte(u8 data, Frame_Receive_buffer * receive_buffer);

/**
 * Returns the number of bytes in `out`
 * if it's 0, it means an error
 */
u16 frame_bytes(u8 * in, u16 in_length, u8 * out, u16 buffer_size, bool add_crc);

/**
 * Calls send_byte for every encoded byte, uses no intermediate buffer
 *
 */
u16 send_on_the_fly(u8 * data, u16 size, void (*send_byte)(u8 byte));

/**
 * Same a send_on_the_fly, but it doesn't send the crc, faster but less relaible
 */
u16 send_on_the_fly_no_crc(u8 * data, u16 size, void (*send_byte)(u8 byte));

/**
 * The state of the crc. This needs to handled separately,
 * otherwise different crc calculations may interfere with each other
 */
typedef u32 Crc_state;

/**
 * Initialize CRC by returning crc state
 */
Crc_state (*crc_init)();

/**
 * This will be used to feed data into the crc calculator
 * This needs to be pointed to a function if sending crc is used
 */
void (*crc_feed)(Crc_state state, u8 data);

/**
 * This variable needs to point to a crc function that returns standard CRC32
 * The string of 8 bytes "HEY SEXY" should calculate 0xAC15FBBF
 */
u32 (*crc_get)(Crc_state);


#endif /* UTIL_DATAFRAME_H_ */
