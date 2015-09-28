# KMS15H3

This is my solution to a homework exercise in the undergrad course "Concepts and Methods of System Software" (KMS) at the University of Paderborn, Germany. It is taught by Prof. Dr. Falko Dressler and mainly deals with operating systems and networking on a low level. Naturally, the programming language C was introduced as well.

The goal of this exercise was, given an MD5 hash, to find its corresponding source string. That string is known to be a concatenation of two words used in "Lés Miserables", separated by a single space. However, as "Lés Miserables" is one of the longest well-known books and contains 570k words, the search space is large. Only considering unique word pairs, it amounts to 10.5 GB. Obviously, a reasonable solution would have to reduce it as much as possible while also optimizing for throughput, in particular using multithreading.

My solution uses the following approach:

- Words are quickly tokenized via strtok, duplicates are removed using a simple djb2 hash table
- Repeated computations (e.g. string length) are avoided, partial data reused where possible to decrease memcpy
- Parallel search on unique word pairs via OpenMP
- Given MD5 implementation improved, later replaced with a manually optimized version
- In general, memory blocks are kept together to avoid cache thrashing
- Various hints to the compiler (const, restrict, etc.) to allow operation ordering

As I felt the solution might be helpful to other students, I put it online. Obviously, by particularly arranging the order of checked words, one could find the target immediately, so my program will continue running even after the hash was found. Compiled with GCC 4.8.1 and running on my i5 quadcore box:

	Loaded 26510 distinct words (194462 b, avg. length: 7.335421 bytes) in 0.070000 seconds
	Performing 702780100 hashes...
	11%, 28.440620 MHs, 427 MB/s
	22%, 28.476925 MHs, 428 MB/s
	33%, 28.492517 MHs, 431 MB/s
	44%, 28.487314 MHs, 430 MB/s
	55%, 28.492512 MHs, 428 MB/s
	66%, 28.495980 MHs, 428 MB/s
	77%, 28.501430 MHs, 426 MB/s
	Found target! "threads help"
	88%, 28.499014 MHs, 426 MB/s
	99%, 28.502917 MHs, 425 MB/s
	Execution took ~25.000000s
	Average word length 7.335421 characters
	Measured clock speed of 3399 MHz in 4 cores
	Performed 702780100 hashes (10502 MB) in 24.683406 seconds
	28.471764 MHs (425 MB/s), 7.117941 MHs per core (106 MB/s), 2.093820 Hs (32.811920 bytes) per kilocycle
