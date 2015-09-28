#ifndef WORDS_H
#define WORDS_H

#include <stdint.h>

typedef struct word_s word;
struct word_s {
	char const * word;
	size_t len;
	word const * next;
};

word const * loadWords (char * restrict source, char const * restrict delim, unsigned int * restrict nwords, size_t * restrict nbytes );

word const * const * convertToArray (word const * head, unsigned int nwords);

/*
Returns 1 if target was found, else 0.
*/
int testHash (const uint8_t buffer [const restrict static 1], unsigned int len);

#endif
