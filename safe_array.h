#ifndef UTIL_SAFE_ARRAY_H_
#define UTIL_SAFE_ARRAY_H_
#include "types.h"

#define foreach(x) for((x)->cursor = 0, (x)->value = &((x)->array[0]); (x)->cursor < (x)->size; (x)->value = &((x)->array[++((x)->cursor)]))
#define loop(index, until) for(u32 index = 0; index < (until); index++)
#define def_array(type, name, size, datatype) datatype    name ## __ ## size ## __buffer [size]; \
                                    type name = {size, 0 , 0, name ## __ ## size ## __buffer}; \
                                    type * name ## _ptr = &name

#define def_char_array(name, size)  def_array(char_array, name, size, char)
#define def_uint16_array(name, size)  def_array(u16_array, name, size, uint16_t)
#define def_uint8_array(name, size)  def_array(u8_array, name, size, uint8_t)
#define def_sint16_array(name, size)  def_array(s16_array,name,size, int16_t)

//Not perfect, but at lest you don't set values that are not part of this array
#define array_set(index, value, arr) if((index) < (arr).size && index >= 0) {(arr).buffer[(index)] = (value);}
#define safe_set(index, value, arr) if((index) < (arr)->size && index >= 0) {(arr)->buffer[(index)] = (value);}
#define array_get(index, arr) ((index < arr.size && index >= 0) ? arr.buffer[index] : 0)


typedef struct {
    size_t size;
    size_t cursor;
    char *value;
    char *buffer;
} char_array;

typedef struct {
    size_t size;
    size_t cursor;
    uint16_t *value;
    uint16_t *buffer;
} u16_array;

typedef struct {
    size_t size;
    size_t cursor;
    uint8_t *value;
    uint8_t *buffer;
} u8_array;

typedef struct {
    size_t size;
    size_t cursor;
    int16_t *value;
    int16_t *buffer;
} s16_array;




#endif /* UTIL_SAFE_ARRAY_H_ */
