#include "dataframe.h"
#include "error_log.h"
#include "safe_array.h"

#include "stm32g070xx.h"
#include "stm32g0xx_ll_crc.h"
#define SEND_ESCAPE(data, needs_escape) \
           switch (data) { \
            case FRAME_START: \
            case FRAME_ESC: \
            case FRAME_END: \
            case FRAME_END_CRC32: \
                needs_escape \
            break; \
            default: \
                break; \
        } \

Frame_Receive_buffer receive_buffer;

typedef u32 Crc_state;
Crc_state (*crc_init)() = NULL;
void (*crc_feed)(Crc_state state, u8 data) = NULL;
u32 (*crc_get)(Crc_state) = NULL;

void reset_dataframe_buffer(Frame_Receive_buffer * frame_receive_buffer, bool start) {
    frame_receive_buffer->buffer_cursor = 0;
    frame_receive_buffer->received_end = false;
    frame_receive_buffer->received_start = start;
    frame_receive_buffer->received_esc = false;
    frame_receive_buffer->buffer_size = FRAME_BUFFER_SIZE;
    frame_receive_buffer->message_size = 0;
    frame_receive_buffer->crc_counter = 0;
    frame_receive_buffer->crc = 0;

    //Not strictly required but makes debugging easier.
    loop(i, FRAME_BUFFER_SIZE)
    {
        frame_receive_buffer->buffer[i] = 0;
    }

}
u8 finished_frames = 0;
/**
 * Returns true if frame ended
 */
bool receive_byte(u8 data, Frame_Receive_buffer * frame_receive_buffer) {
    if (frame_receive_buffer->received_start && !frame_receive_buffer->received_end) {

        if (frame_receive_buffer->crc_counter) {
            frame_receive_buffer->crc_counter--;
            frame_receive_buffer->crc = frame_receive_buffer->crc << 8;
            frame_receive_buffer->crc |= data;
            if (!frame_receive_buffer->crc_counter) {
                finished_frames++;
                frame_receive_buffer->received_end = true;

                Crc_state state = crc_init();
                loop(i, frame_receive_buffer->buffer_cursor)
                {
                    crc_feed(state, frame_receive_buffer->buffer[i]);
                }

                u32 crc_result = crc_get(state);
                if (crc_result != frame_receive_buffer->crc) {
                    log_error(EC_FRAME_RECEIVE_CRC_MISMATCH, finished_frames);

                }
                frame_receive_buffer->received_start = false;
                frame_receive_buffer->received_esc = false;
                frame_receive_buffer->message_size = frame_receive_buffer->buffer_cursor;
                return true;
            }
        }
        else if (!frame_receive_buffer->received_esc) {
            if (data == FRAME_START) {
                log_error(EC_FRAME_RECEIVED_START_WHILE_MESSAGE_STILL_IN_PROGRESS, finished_frames);
                //On error, just ignore the current frame and start again
                reset_dataframe_buffer(frame_receive_buffer, true);
            } else if (data == FRAME_END_CRC32) {
                frame_receive_buffer->crc_counter = 4;
                //Receive CRC, and than transmission is finished
            } else if (data == FRAME_END) {
                //End transmission
                finished_frames++;
                frame_receive_buffer->received_end = true;
                frame_receive_buffer->received_start = false;
                frame_receive_buffer->received_esc = false;
                frame_receive_buffer->message_size = frame_receive_buffer->buffer_cursor;
                return true;
            } else if (data == FRAME_ESC) {
                frame_receive_buffer->received_esc = true;
            } else {
                //Regular byte
                frame_receive_buffer->buffer[frame_receive_buffer->buffer_cursor] = data;
                frame_receive_buffer->buffer_cursor++;
            }
        } else {
            frame_receive_buffer->buffer[frame_receive_buffer->buffer_cursor] = data;
            frame_receive_buffer->buffer_cursor++;
            //Once we received the escaped byte we reset the escaping
            frame_receive_buffer->received_esc = false;
        }
    } else if (data == FRAME_START) {
        //TODO maybe check if previous finished
        reset_dataframe_buffer(frame_receive_buffer, true);
    } else {
        log_error(EC_FRAME_RECEIVED_UNEXPECTED_BYTE, finished_frames);

    }

    return false;
}

u16 send_on_the_fly_no_crc(u8 * data, u16 size, void (*send_byte)(u8 byte, u8 skip_port),
        u8 skip_port) {
    send_byte(FRAME_START, skip_port);

    loop(i, size)
    {
        u8 cb = data[i];
        SEND_ESCAPE(cb, send_byte(FRAME_ESC,skip_port););
        send_byte(cb, skip_port);
    }
    send_byte(FRAME_END, skip_port);

    return size;
}
u16 send_on_the_fly(u8 * data, u16 size, void (*send_byte)(u8 byte, u8 skip_port), u8 skip_port) {
    /** Check is CRC functions are set **/
    if (crc_feed == NULL || crc_get == NULL) {
        log_error(EC_FRAME_SEND_CRC_FUNCTIONS_NOT_SET, 'f');
        send_on_the_fly_no_crc(data, size, send_byte, skip_port);
    }

    /** Send start of the frame byte **/
    send_byte(FRAME_START, skip_port);
    Crc_state state = crc_init();
    loop(i, size)
    {
        u8 cb = data[i];
        //If it's an escaped character, send an escape byte before sending the data;
        SEND_ESCAPE(cb, send_byte(FRAME_ESC, skip_port););
        /** Send byte through the arbitrary send function **/
        send_byte(cb, skip_port);
        /** Feed each byte to the crc calculator function **/
        crc_feed(state, cb);

    }

    /** Send end of frame with a 32 bit crc **/
    send_byte(FRAME_END_CRC32, skip_port);
    /** Get result from crc calculator**/
    u32 crc = crc_get(state);
    /** Send crc, MSB first **/
    send_byte(crc >> 24 & 0xFF, skip_port);
    send_byte((crc >> 16) & 0xFF, skip_port);
    send_byte((crc >> 8) & 0xFF, skip_port);
    send_byte(crc & 0xFF, skip_port);

    return size;
}

u16 frame_bytes(u8 * in, u16 in_length, u8 * out, u16 buffer_size, bool add_crc) {
    u16 out_count = 0;
    /** The maximum the buffer should be filled while sending that,
     * this includes the 2 bytes for escaping, and if needed 5 bytes for end and crc
     * or 1 byte for end frame **/
    const u16 out_limit = buffer_size - 2 - (add_crc ? 5 : 1);
    out[out_count++] = FRAME_START;

    Crc_state state = 0;
    if (add_crc) {
        state = crc_init();
    }

    loop(i, in_length)
    {
        if (out_count >= out_limit) {
            log_error(EC_FRAME_SEND_BUFFER_OVERFLOW, 'f');
            return 0;
        }

        u8 cb = in[i];
        SEND_ESCAPE(cb, out[out_count++] = FRAME_ESC;);
        out[out_count++] = in[i];

        /** TODO, this maybe should not be in an if **/
        if (add_crc && crc_feed && crc_get) {
            crc_feed(state, cb);
        }
    }
    if (add_crc) {
        out[out_count++] = FRAME_END_CRC32;
        /** Get result from crc calculator**/
        u32 crc = crc_get(state);
        /** Send crc, MSB first **/
        out[out_count++] = (crc >> 24 & 0xFF);
        out[out_count++] = ((crc >> 16) & 0xFF);
        out[out_count++] = ((crc >> 8) & 0xFF);
        out[out_count++] = (crc & 0xFF);
    } else {
        out[out_count++] = FRAME_END;
    }

    return out_count;
}

/**
 * TODO do something to run these automatically.
 */
void unit_test() {
    Frame_Receive_buffer bfr;
    reset_dataframe_buffer(&bfr, false);
    u8 * abc = (u8 *) "HEY SEXY";
    u8 zzz[64] = { 0 };

    u16 wrote_bytes = frame_bytes(abc, 8, zzz, 64, true);
    if (wrote_bytes != 14) {
        // failed
    }

    loop(i, wrote_bytes)
    {
        //This should run without a problem
        receive_byte(zzz[i], &bfr);
    }

    loop(i,8)
    {
        if (abc[i] != bfr.buffer[i]) {
            //Test failed
        }
    }

    zzz[4] = 'z';

    loop(i, wrote_bytes)
    {
        //This should generate a crc mismatch
        receive_byte(zzz[i], &bfr);
    }

}
