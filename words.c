
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "words.h"
#include "md5.h"
#include "fastmd5.h"

#define HASH_LENGTH 16

word const * addWordToChain (word const * restrict chain, char const * restrict add, size_t len)
{
	word * w = malloc(sizeof(word));
	w->word = add;
	w->len = len;
	w->next = chain;
	return w;
}

unsigned int djb2 (uint8_t const * const restrict str, size_t len)
{
	unsigned int hash = 5381;
	for (int i = 0; i < len; i++) {
		int c = str[i];
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

// Word lookup: 16 bit hashtable. Small and very fast.
enum { HashtableSize = 1024 * 64 };

/*
Returns 0 if word was unknown.
Word will be added to (lookup) list of known words if unknown.
*/
int checkKnownWord (word const * lookup [const restrict static HashtableSize], char const * restrict p, size_t len) {
	unsigned int hash = djb2 ((uint8_t*)p, len);
	hash %= HashtableSize;
	const word * restrict check = lookup[hash];
	while (check != 0) {
		if (!strcmp (check->word, p)) {
			return 1;
		}
		check = check->next;
	}

	lookup[hash] = addWordToChain (lookup[hash], p, len);
	return 0;
}

word const * loadWords (char * restrict source, char const * restrict delim, unsigned int * restrict nwords, size_t * restrict nbytes )
{
	*nwords = 0;
	*nbytes = 0;
	word const * head = 0;
	word const * lookup [HashtableSize] = {0};
	
	char * parse = strtok (source, delim);
	while (parse != 0) {
		int len = strlen (parse);
		if (!checkKnownWord (lookup, parse, len)) {
			head = addWordToChain (head, parse, len);
			(*nwords)++;
			(*nbytes) += head->len;
		}
		parse = strtok (0, delim);
	}
	return head;
}

word const * const * convertToArray (word const * head, unsigned int nwords)
{
	word const * * list = malloc( sizeof(word*) * nwords );
	word const * p = head;
	for (unsigned int i = 0; i < nwords; i++) {
		list[nwords - i - 1] = p;
		p = p->next;
	}
	return list;
}

/*
Returns 1 if target was found, else 0.
*/
int testHash (const uint8_t buffer [const restrict static 1], unsigned int len)
{
	uint8_t hash[HASH_LENGTH] = {0};
	if (0) {
		// provided MD5 implementation
		MD5_CTX ctx;
		MD5_Init (&ctx);
		MD5_Update (&ctx, buffer, len);
		MD5_Final (hash, &ctx);
	} else {
		// custom MD5 implementation
		md5_hash (buffer, len, hash);
	}
	
	const uint8_t target[HASH_LENGTH] = {0xe6, 0x54, 0x93, 0xcc, 0xde, 0xe9, 0xc4, 0x51, 0x4f, 0xe2, 0x0e, 0x04, 0x04, 0xf3, 0xbc, 0xb8}; // "???" - to be used for les_miserables.txt
	//const uint8_t target[HASH_LENGTH] = {0x1c, 0x01, 0x43, 0xc6, 0xac, 0x75, 0x05, 0xc8, 0x86, 0x6d, 0x10, 0x27, 0x0a, 0x48, 0x0d, 0xec}; // "Les Miserables" - to be used for les_miserables_preface.txt
	
	return !memcmp (hash, target, HASH_LENGTH);
}
