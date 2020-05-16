/**
 * Defines the stdout function for the unit test API. This is in the interest
 * of increased portability for compilers with incomplete STLs, e.g. Arduino.
 */

 #include <stdio.h>

#define PRINTF(kFmt, ...) printf (kFmt, ##__VA_ARGS__);