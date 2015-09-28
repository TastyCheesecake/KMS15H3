#ifndef FASTMD5_H
#define FASTMD5_H

#include <stdint.h>

void md5_hash( const uint8_t message [const restrict], uint32_t len, uint8_t thash [restrict static 16]);

#endif
