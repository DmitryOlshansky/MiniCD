#ifdef MINICD
#include "minicd.h"

#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
typedef long long int clock_t;
#define CLOCKS_PER_SEC __get_CLOCKS_PER_SEC()
clock_t __get_CLOCKS_PER_SEC()
{	
	union{
		clock_t t;
		LARGE_INTEGER li;
	};
	QueryPerformanceFrequency(&li);
	return t;
}

clock_t clock()
{
	union{
		clock_t t;
		LARGE_INTEGER li;
	};
	QueryPerformanceCounter(&li);
	return t;
}

#endif

void print_stats(clock_t tv0)
{
	clock_t t, scale;

	t = clock();
	t -= tv0;
	scale = (CLOCKS_PER_SEC+500)/1000; //ticks per milisecond
	printf("%ld,%03ld\n", (t*1000000/scale)/1000, (t*1000000/scale)%1000);
}

void run_bench(const char *label, size_t (*bench)(void *), void *params)
{
	clock_t tv0;
	//printf("%s\n\t", label);
	tv0 = clock();
	bench(params);
	print_stats(tv0);
}

#define RUN(a, b) \
	size_t (a)(void *); \
	run_bench(#a " (" #b ")", (a), (b))

int main()
{
	clock_t t;
	t = clock();
	RUN(b_malloc_sparse, 0);
	RUN(b_malloc_bubble, 0);
	RUN(b_malloc_tiny1, 0);
	RUN(b_malloc_tiny2, 0);
	RUN(b_malloc_big1, 0);
	RUN(b_malloc_big2, 0);
/*	RUN(b_malloc_thread_stress, 0);
	RUN(b_malloc_thread_local, 0);
*/
	RUN(b_string_strstr, "abcdefghijklmnopqrstuvwxyz");
	RUN(b_string_strstr, "azbycxdwevfugthsirjqkplomn");
	RUN(b_string_strstr, "aaaaaaaaaaaaaacccccccccccc");
	RUN(b_string_strstr, "aaaaaaaaaaaaaaaaaaaaaaaaac");
	RUN(b_string_strstr, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac");
	RUN(b_string_memset, 0);
	RUN(b_string_strchr, 0);
	RUN(b_string_strlen, 0);
/*
	RUN(b_pthread_createjoin_serial1, 0);
	RUN(b_pthread_createjoin_serial2, 0);
	RUN(b_pthread_create_serial1, 0);
	RUN(b_pthread_uselesslock, 0);

	RUN(b_utf8_bigbuf, 0);
	RUN(b_utf8_onebyone, 0);

	RUN(b_stdio_putcgetc, 0);
	RUN(b_stdio_putcgetc_unlocked, 0);

	RUN(b_regex_compile, "(a|b|c)*d*b");
	RUN(b_regex_search, "(a|b|c)*d*b");
	RUN(b_regex_search, "a{25}b");
*/
}

