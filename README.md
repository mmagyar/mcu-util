# mcu_util
A collection of useful, platform independent utilities in C99, targeting mainly microcontrollers

# Constrains of these utilities
 - No dynamic memory allocation
 - As little as posible static ram usage.
 - No float usage

# Provided utilites
 - Simple error logging functionality with minimal memory usage
 - Array helper functions and a safe array implementation with foreach, safe value setting and getting with bounds checking, error reporting
 - A circular buffer implementation, with error feedback on overflow, usefule for RX and TX FIFO implementation
 - Simple graphics drawing functions
 - A 5x7 Bitmap font in a header
 - Type definitions to simplify primitive types (ex: u16 instead of unsigned short or uint16_t)
 
# Code style
 - Variable names are snake_case
 - Struct name are Snake_case
 - Enum members are SNAKE_CASE
 - Defines are SNAKE_CASE
 
# Usage
 Clone the repository directly in to your src folder, and include the needed header files, or include system_defs.h to include everythig
 
# Dependencies

 The only dependencies are the most basic parts of the standrad c library:
    - stdint.h
	- stdbool.h
	- stddef.h
	
#License
  Licensed under the MIT license