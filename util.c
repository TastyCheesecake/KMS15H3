#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "util.h"

char * ReadFile (char const * path)
{
	FILE * f = fopen (path, "rb"); // rb instead of r as proposed by instructor to avoid size issues
	if (!f) {
		printf("Could not open file!");
		exit(1);
	}
	
	fseek (f, 0, SEEK_END);
	size_t len = ftell (f);
	rewind(f);
	
	char* buffer = malloc (sizeof(char) * (len + 1));
	size_t read = fread (buffer, sizeof(char), len, f);
	if (read != len) {
		exit(1);
	}
	buffer[len] = '\0';
	
	return buffer;
}

double getTimeDelta (const struct timeval * tStart)
{
	struct timeval tEnd;
	gettimeofday (&tEnd, NULL);
	double delta = 1.0 * (tEnd.tv_sec - tStart->tv_sec) + 1.0/1000/1000 * (tEnd.tv_usec - tStart->tv_usec);
	return delta;
}

uint64_t rdtsc()
{
	uint64_t t;
	__asm volatile ("rdtsc" : "=A" (t));
	return t;
}
