#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>

int logmsgf(const char *fmt, ...);
int dump_array_int16_t(const char* name, int16_t* s, size_t n);
#endif
