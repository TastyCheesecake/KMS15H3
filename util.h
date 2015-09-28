#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

char * ReadFile(char const * path);

double getTimeDelta (const struct timeval * tStart);

uint64_t rdtsc();

#endif
