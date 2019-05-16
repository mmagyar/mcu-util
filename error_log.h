#ifndef UTIL_ERROR_LOG_H_
#define UTIL_ERROR_LOG_H_
#include <stdint.h>
#include <stdbool.h>

#ifndef ERROR_LOG_SIZE
#define ERROR_LOG_SIZE 16
#endif


typedef enum {
    EC_NO_ERROR = 0x00,
    EC_CIRCULAR_BUFFER_ALLOCATE_FAIL = 0x01,
    EC_CIRCULAR_BUFFER_OVERFLOW = 0x02,
    EC_PRINT_BUFFER_FULL = 0x03,

    EC_UART_1_OVERFLOW = 0x11,
    EC_UART_1_FRAMING_ERROR = 0x12,
    EC_UART_1_NOISE_ERROR = 0x13,
    EC_UART_1_UNEXPECTED = 0x1F,
    EC_UART_2_OVERFLOW = 0x21,
    EC_UART_2_FRAMING_ERROR = 0x22,
    EC_UART_2_NOISE_ERROR = 0x23,
    EC_UART_2_UNEXPECTED = 0x2F,
    EC_UART_3_OVERFLOW = 0x31,
    EC_UART_3_FRAMING_ERROR = 0x32,
    EC_UART_3_NOISE_ERROR = 0x33,
    EC_UART_3_UNEXPECTED = 0x3F,
    EC_UART_4_OVERFLOW = 0x41,
    EC_UART_4_FRAMING_ERROR = 0x42,
    EC_UART_4_NOISE_ERROR = 0x43,
    EC_UART_4_UNEXPECTED = 0x4F,
    EC_SPI_1_ERROR = 0x51,
    EC_SPI_1_CRC_ERROR = 0x52,
    EC_SPI_1_FRAME_ERROR = 0x53,
    EC_SPI_1_OVERFLOW = 0x54,
    EC_SPI_1_MODE_FAULT = 0x55,
    EC_SPI_1_UNHANDLED_TX_IRQ = 0x56,
    EC_SPI_1_UNEXPECTED = 0x5F,
    EC_SPI_2_ERROR = 0x61,
    EC_SPI_2_CRC_ERROR = 0x62,
    EC_SPI_2_FRAME_ERROR = 0x63,
    EC_SPI_2_OVERFLOW= 0x64,
    EC_SPI_2_MODE_FAULT = 0x65,
    EC_SPI_2_UNHANDLED_TX_IRQ = 0x66,
    EC_SPI_2_UNEXPECTED = 0x6F,

    EC_MIDI_UNKNOWN_ERROR = 0x80,
    EC_MIDI_EMPTY_MESSAGE_TYPE = 0x81,

    //Previous message was not finished
    EC_MIDI_UNFINISHED_MESSAGE = 0x82,

    //Data receiving state machine is in an invalid state
    EC_MIDI_INVALID_STATE = 0x83,

    EC_FRAME_RECEIVED_START_WHILE_MESSAGE_STILL_IN_PROGRESS = 0x90,
    EC_FRAME_RECEIVED_UNEXPECTED_BYTE = 0x91,
    EC_FRAME_SEND_CRC_FUNCTIONS_NOT_SET = 0x92,
    EC_FRAME_SEND_BUFFER_OVERFLOW = 0x93,
    EC_FRAME_RECEIVE_CRC_MISMATCH = 0x94,
    EC_COMM_DROPPED_MESSAGE = 0xA0,
    EC_COMM_PEER_LIMIT_EXCEEDED = 0xA1,
    EC_COMM_UNKOWN_MESSAGE_STRUCTURE = 0xA2,

} Error_codes;

/**
 * errorCode:
 * 0x02 - Circular buffer overflow
 *
 * identifier:
 * in case of buffer error, it contains the buffers identifier
 */
typedef struct {
    Error_codes error_code;
    char identifier;
} Error;

unsigned int error_count;
void log_error_direct(Error error);
void log_error(Error_codes, char identifier);
Error error_log[ERROR_LOG_SIZE];

#endif /* UTIL_ERROR_LOG_H_ */
