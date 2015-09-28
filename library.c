#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#include <stdint.h>
#include <sys/time.h>

#include "words.h"
#include "util.h"

// delim contains \n, \r and \t in addition
const char * const delim = "\n\r\t .,;-:0123456789?!\"*+()|&[]#$/%%'";

int main () {
	printf( "\n" );
	printf( "LIBRARY [KMS H3 2015]\n" );
	printf( "Written by Sebastian Heuchler, sh87 at mail dot upb dot de\n" );
	printf( "Source code available at http://kikashi.net/ugc/kms\n" );
	time_t start = time( NULL );
	struct timeval tStart;

	/*
	Read source file
	*/
	gettimeofday( &tStart, NULL );
	char* source = ReadFile( 1
		? "les_miserables.txt"
		: "les_miserables_preface.txt"
		);
	unsigned int nwords;
	size_t nbytes;
	const word* head = loadWords( source, delim, &nwords, &nbytes );
	
	/*
	Convert to array to make OMP happy
	*/
	double avgWordLength = 1.0 * nbytes / nwords;
	const word* const* list = convertToArray( head, nwords );
	printf( "Loaded %u distinct words (%u b, avg. length: %f bytes) in %f seconds\n", nwords, nbytes, avgWordLength, getTimeDelta( &tStart ));

	/*
	Parallel search
	*/
	printf( "Performing %u hashes...\n", nwords * nwords );
	gettimeofday( &tStart, NULL );
	uint64_t clk = rdtsc();
	unsigned done = 0;
	unsigned nprint = nwords / 9 - 1;
	unsigned totalhashes = 0;
	uint64_t totalbytes = 0;
	int found = 0;
	
	#pragma omp parallel for
	for (int i = 0; i < nwords; i++) {
		if (found) continue;
		
		uint8_t buffer[256];
		uint64_t add_bytes = 0;
		
		const word* p1 = list[i];
		memcpy( buffer, p1->word, p1->len );
		buffer[p1->len] = ' ';
		size_t bufferUsedLen = p1->len + 1;
		
		const word* p2 = head;
		while (p2 != 0) {
			memcpy( buffer + bufferUsedLen, p2->word, p2->len );
			unsigned len = bufferUsedLen + p2->len;
			add_bytes += len;
			if (testHash( buffer, len )) {
				buffer[bufferUsedLen + p2->len] = '\0';
				printf( "Found target! \"%s\"\n", buffer );
				//found = 1; break;
			}
			p2 = p2->next;
		}
		
		#pragma omp atomic
		done++;
		#pragma omp atomic
		totalhashes += nwords;
		#pragma omp atomic
		totalbytes += add_bytes;
		if (done % nprint == 0) {
			double delta = getTimeDelta( &tStart );
			unsigned perc = 100 * done / nwords;
			double hs = 1.0 * done * nwords / delta;
			double bs = 1.0 * totalbytes / delta;
			printf( "%u%%, %f MHs, %u MB/s\n", perc, hs/1000/1000, (unsigned)(bs/1024/1024) );
		}
	}

	double delta = getTimeDelta( &tStart );
	time_t end = time( NULL );
	clk = rdtsc() - clk;
	
	unsigned cores = omp_get_num_procs ();
	double hz  = 1.0 * clk / delta;
	double hs  = totalhashes / delta;
	double mhs = hs / 1000 / 1000;
	double bs  = totalbytes / delta;
	double mbs = bs / 1024 / 1024;
	
	printf( "Execution took ~%fs\n", difftime( end, start ) );
	printf( "Measured clock speed of %u MHz in %u cores\n", (unsigned)(hz/1000/1000), cores );
	printf( "Performed %u hashes (%u MB) in %f seconds\n", (unsigned)totalhashes, (unsigned)(totalbytes / 1024 / 1024), delta );
	printf( "%f MHs (%u MB/s), %f MHs per core (%u MB/s), %f Hs (%f bytes) per kilocycle\n",
		mhs,
		(unsigned)mbs,
		mhs / cores,
		(unsigned)(mbs / cores),
		1000 * hs / cores / hz,
		1000 * bs / cores / hz
		);
	
	return 0;
}
