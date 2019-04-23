#ifndef UTIL_ERROR_LOG_CODE_TEXT_H_
#define UTIL_ERROR_LOG_CODE_TEXT_H_

/**
 * This array contains the name of all the error code as strings
 * This take 16kb of memory - most compiler will store this in flash only.
 * If your mcu does not have that much free flash memory, this file can be safely ignored,
 * the only side effect is that you can't print the error names
 */
const char error_code_text[0xFF][64];

#endif /* UTIL_ERROR_LOG_CODE_TEXT_H_ */
