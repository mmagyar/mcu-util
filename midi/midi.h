#ifndef MIDI_H_
#define MIDI_H_
#include <util.h>
#include <stdint.h>
#include <stdbool.h>
#include "./midi_enums.h"
#include <util/circular_buffer.h>
//Size of the midi buffer, in powers of 2
#define MIDI_BUFFER_SIZE 4

extern Circular_buffer midiRXBuffer;
extern Circular_buffer midiTXBuffer;

/**
 * System exclusive message buffer in powers of 2 4 = 16
 */
//#define bufferSysEx 4

#ifdef bufferSysEx
extern Circular_buffer sys_ex_buffer;
#else
#endif

typedef struct {
    uint8_t channel;
    uint8_t cc_number;
    uint16_t cc_value;
} MidiControlChange;
typedef uint16_t Midi_cc_value;

void send_cc(Midi_channel channel, Midi_cc_number cc, uint8_t value);

/**
 * This struct is used to track the currently receiving message
 */
typedef struct {
    RECEIVE_STATUS receive_status;
    Midi_channel channel;
    Midi_note note;
    Midi_cc_number cc_number;
    uint16_t pitchwheel;
    Midi_message_type message_type;
    bool in_sysex;
} Current_message;

typedef void (*Midi_note_event_cb)(Midi_channel channel, Midi_note note, Note_event_type type, uint8_t value);
typedef void (*Midi_cc_change_cb)(Midi_channel channel, Midi_cc_number number, Midi_cc_value value);
typedef void (*Midi_program_change_cb)(Midi_channel channel, uint8_t value);
typedef void (*Midi_pitch_wheel_cb)(Midi_channel channel, uint16_t value);
typedef void (*Midi_channel_pressure_cb)(Midi_channel channel, uint8_t value);
typedef void (*Midi_sys_ex_byte_cb)(SysExStatus status, uint8_t value);
typedef void (*Midi_sys_ex_buffer_cb)(SysExStatus status, Circular_buffer *value);

extern Midi_note_event_cb note_event_callback;
extern Midi_cc_change_cb cc_change_callback;
extern Midi_program_change_cb program_change_callback;
extern Midi_pitch_wheel_cb pitch_wheel_callback;
extern Midi_channel_pressure_cb channel_pressure_callback;

#ifndef bufferSysEx
extern Midi_sys_ex_byte_cb sys_ex_byte_callback;
#endif

/**
 * Buffer needs to be copied immediately, since it will be reused once the function returns
 */
#ifdef bufferSysEx
extern Midi_sys_ex_buffer_cb sys_ex_buffer_callback;
#endif

#endif /* MIDI_H_ */
