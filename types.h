#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef float f32;
typedef double f64;




typedef union u4 {
	struct {
		u8 low : 4;
		u8 high : 4;
	} nibbles_4bit;
	u8 byte;
} u4;

typedef union u2 {
	struct {
		u8 b0 : 2;
		u8 b1 : 2;
		u8 b2 : 2;
		u8 b3 : 2;
	} nibbles_2bit;
	u8 byte;
} u2;

typedef union u1 {
	struct {
		u8 b0 : 1;
		u8 b1 : 1;
		u8 b2 : 1;
		u8 b3 : 1;
		u8 b4 : 1;
		u8 b5 : 1;
		u8 b6 : 1;
		u8 b7 : 1;
	} nibbles_1bit;
	u8 byte;
} u1;
