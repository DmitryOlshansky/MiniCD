/**
	When used instead of MSVC++ CRT keep in mind that
	MiniCD is not full CRT = NO GLOBALs & STATICs with constructors/destructors 
*/

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//imports
#ifdef __cplusplus
extern "C" {
#endif

extern unsigned memory_page_size;

//stdio.h
#define EOF (-1)
#define BUFSIZ (memory_page_size*8)
typedef unsigned long long fpos_t;

enum { STDIO_READ = 1, STDIO_WRITE = 2, STDIO_APPEND = 4 };
typedef struct tagFILE{
	//TODO: union some fields, honor r/w/a/+ strings more strictly
	HANDLE file, mapping;//file + memory mapping
	unsigned char *bstart, *bcur, *bend;//mapping range, bcur - cur pos in view
	//buffer_size - mapping window size
	size_t buffer_size;//pos inside of buffer, ofs not updated on getc/fgetc/putc...
	char  mode;//mode flags
	int unget_;
	fpos_t ofs, file_size;	//ofs - real position in file, synced on buffer ops & seek
	//file_size - full size of file 
} FILE;

//file-level operations
/**
	The remove function causes the file whose name is the string 
	pointed to by filename to be no longer accessible by that name.
*/
int remove(const char *filename);

/**
	The rename function causes the file whose name is the string pointed to by old_name 
	to be henceforth known by the name given by the string pointed to by new_name. 
*/
int rename(const char *old_name, const char *new_name);

/**
	The tmpfile function creates a temporary binary file that is different from any other
	existing file and that will automatically be removed when it is closed or at program
	termination.
*/
FILE *tmpfile(void);

/**
	The tmpnam function generates a string that is a valid file name and that is not the same
	as the name of an existing file.
*/
char *tmpnam(char *s);

//Open file with mode 'mode' and return pointer to its FILE object.
FILE* fopen(const char* path, const char* mode);

//Release resource of FILE object f.
void fclose(FILE* f);

int fgetc(FILE * f);

int fputc(int ch, FILE *f);

int ungetc(int ch, FILE* f);

int feof(FILE* f);

//printf family
int __declspec(dllimport) sprintf(char* dest, const char* fmt, ...);
int __declspec(dllimport) _snprintf(char* dest, size_t len, const char* fmt, ...);
int __declspec(dllimport) _vsnprintf(char* dest, size_t len, const char* fmt, va_list arg);

//forwarded, reimplemented etc.
int printf(const char* fmt, ...);

//"user-mode" entry point
int main(int argc, char* argv[]);

//ctype.h

///isalpha(c) or isdigit(c)
int __declspec(dllimport)isalnum(int c);
///isupper(c) or islower(c)
int __declspec(dllimport)isalpha(int c);
///is control character
int __declspec(dllimport)iscntrl(int c);
///is decimal digit
int __declspec(dllimport)isdigit(int c);
///is printing character other than space
int __declspec(dllimport)isgraph(int c);
///is lower-case letter
int __declspec(dllimport)islower(int c);
///is printing character (including space)
int __declspec(dllimport)isprint(int c);
///is printing character other than space, letter, digit
int __declspec(dllimport)ispunct(int c);
///is space, formfeed, newline, carriage return, tab, vertical tab
int __declspec(dllimport)isspace(int c);
///is upper-case letter
int __declspec(dllimport)isupper(int c);
///is hexadecimal digit
int __declspec(dllimport)isxdigit(int c);
///return lower-case equivalent
int __declspec(dllimport)tolower(int c);
///return upper-case equivalent
int __declspec(dllimport)toupper(int c);

//stdlib.h

///Returns numerical value of s. Equivalent to strtod(s, (char**)NULL).
//double __declspec(dllimport)  atof(const char* s);
///Returns numerical value of s. Equivalent to (int)strtol(s, (char**)NULL, 10).
int __declspec(dllimport)  atoi(const char* s);
///Returns numerical value of s. Equivalent to strtol(s, (char**)NULL, 10).
long __declspec(dllimport)  atol(const char* s);
///Converts prefix of s to double, ignoring leading quite space. Stores a pointer to any unconverted suffix in *endp if endp non-NULL. If answer would overflow, HUGE_VAL is returned with the appropriate sign; if underflow, zero returned. In either case, errno is set to ERANGE.
//double __declspec(dllimport)  strtod(const char* s, char** endp);
///Converts prefix of s to long, ignoring leading quite space. Stores a pointer to any unconverted suffix in *endp if endp non-NULL. If base between 2 and 36, that base used; if zero, leading 0X or 0x implies hexadecimal, leading 0implies octal, otherwise decimal. Leading 0X or 0x permitted for base 16. If answer would overflow, LONG_MAX or LONG_MIN returned and errno is set to ERANGE.
long __declspec(dllimport)  strtol(const char* s, char** endp, int base);
///As for strtol except result is unsigned long and error value is ULONG_MAX.
unsigned __declspec(dllimport)  long strtoul(const char* s, char** endp, int base);
///Returns pseudo-random number in range 0 to RAND_MAX.
int __declspec(dllimport)  rand();
///Uses seed as seed for new sequence of pseudo-random numbers. Initial seed is 1.
void __declspec(dllimport)  srand(unsigned int seed);
///Returns pointer to zero-initialised newly-allocated space for an array of nobj objects each of size size, or NULL if request cannot be satisfied.
void* calloc(size_t nobj, size_t size);
///Returns pointer to uninitialised newly-allocated space for an object of size size, or NULL if request cannot be satisfied.
void* malloc(size_t size);
///Changes to size the size of the object to which p points. Contents unchanged to minimum of old and new sizes. If new size larger, new space is uninitialised. Returns ponter to the new space or, if request cannot be satisfied NULL leaving p unchanged.
void* realloc(void* p, size_t size);
///Deallocats space to which p points. p must be NULL, in which case there is no effect, or a pointer returned by calloc, malloc or realloc.
void  free(void* p);
///Causes program to terminate abnormally.
void abort();
/**
	Causes normal program termination. 
	Functions installed using atexit are called in reverse order of registration, open files are flushed, 
	open streams are closed and control is returned to environment. status is returned to environment in implementation-dependent manner. 
	Zero indicates successful termination and the values EXIT_SUCCESS and EXIT_FAILURE may be used.
*/
void exit(int status);
///Registers fcn to be called when program terminates normally. Non-zero returned if registration cannot be made.
int atexit(void (*fcm)());
///Passes s to environment for execution. If s is NULL, non-zero returned if command processor exists; return value is implementation-dependent if s is non-NULL.
int  system(const char* s);
///Returns (implementation-dependent) environment string associated with name, or NULL if no such string exists.
const char* getenv(const char* name);
///Searches base[0]...base[n-1] for item matching *key. Comparison function cmp must return negative if first argument is less than second, zero if equal and positive if greater. The n items of base must be in ascending order. Returns a pointer to the matching entry or NULL if not found.
void __declspec(dllimport)  bsearch(const void* key, const void* base, size_t n, size_t size, int (*cmp)(const void* keyval, const void* datum));
///Arranges into ascending order the array base[0]...base[n-1] of objects of size size. Comparison function cmp must return negative if first argument is less than second, zero if equal and positive if greater.
void __declspec(dllimport)  qsort(void* base, size_t n, size_t size, int (*cmp)(const void*, const void*));
///Returns absolute value of n
int __declspec(dllimport)  abs(int n);
///Returns absolute value of n
long __declspec(dllimport)  labs(long n);
///Returns in fields quot and rem of structure of type div_t the quotient and remainder of num/denom.
//div_t __declspec(dllimport)  div(int num, int denom);
///Returns in fields quot and rem of structure of type ldiv_t the quotient and remainder of num/denom.
//ldiv_t __declspec(dllimport)  ldiv(long num, long denom);

//string.h

///Copy ct to s including terminating NUL. Return s.
char* strcpy(char* s, const char* ct);
///Copy at most n characters of ct to s Pad with NULs if ct is of length less than n. Return s.
//__declspec(dllimport) char* strncpy(char* s, const char* ct, int n);
///Concatenate ct to s. Return s.
//__declspec(dllimport) char* strcat(char* s, const char* ct);
///Concatenate at most n characters of ct to s. Terminate s with NUL and return it.
//__declspec(dllimport) char* strncat(char* s, const char* ct, int n);
///Compare cs and ct. Return negative if cs < ct, zero if cs == ct, positive if cs > ct.
//__declspec(dllimport) int strcmp(const char* cs, const char* ct);
///Compare at most n characters of cs and ct. Return negative if cs < ct, zero if cs == ct, positive if cs > ct.
//int __declspec(dllimport) strncmp(const char* cs, const char* ct, int n);
///Return pointer to first occurrence of c in cs, or NULL if not found.
//__declspec(dllimport) char* strchr(const char* cs, int c);
///Return pointer to last occurrence of c in cs, or NULL if not found.
//__declspec(dllimport) char* strrchr(const char* cs, int c);
///Return length of prefix of cs consisting entirely of characters in ct.
__declspec(dllimport) size_t strspn(const char* cs, const char* ct);
///Return length of prefix of cs consisting entirely of characters not in ct.
__declspec(dllimport) size_t strcspn(const char* cs, const char* ct);
///Return pointer to first occurrence within cs of any character of ct, or NULL if not found.
//__declspec(dllimport) char* strpbrk(const char* cs, const char* ct);
///Return pointer to first occurrence of ct in cs, or NULL if not found.
//__declspec(dllimport) char* strstr(const char* cs, const char* ct);
///Return length of cs.
__declspec(dllimport) size_t strlen(const char* cs);
///Return pointer to implementation-defined string corresponding with error n.
//__declspec(dllimport) const char* strerror(int n);
/**
	A sequence of calls to strtok returns tokens from s delimted by a character in ct.
	Non-NULL s indicates the first call in a sequence. ct may differ on each call. 
	Returns NULL when no such token found.
*/
__declspec(dllimport)char* strtok(char* s, const char* t);
///Copy n characters from ct to s. Return s. Does not work correctly if objects overlap.
//__declspec(dllimport)void* memcpy(void* s, const void* ct, int n);
///Copy n characters from ct to s. Return s. Works correctly even if objects overlap.
// __declspec(dllimport) void* memmove(void* s, const void* ct, int n);
/**
	Compare first n characters of cs with ct. 
	Return negative if cs < ct, zero if cs == ct, positive if cs > ct.
*/
//int __declspec(dllimport) memcmp(const void* cs, const void* ct, int n);
/**
	Return pointer to first occurrence of c in first n characters of cs, 
	or NULL if not found.
*/


//time.h
//An arithmetic type elapsed processor representing time.
typedef long long int clock_t;
typedef long long int time_t;
///Returns elapsed processor time used by program or -1 if not available.
clock_t clock();
///The number of clock_t units per second.
#define CLOCKS_PER_SEC __get_CLOCKS_PER_SEC()
clock_t __get_CLOCKS_PER_SEC();
/**
	Get the current calendar time as a time_t object.
	If the argument is not a null pointer, the return value is the same
	as the one stored in the location pointed by the argument.
*/
time_t time(time_t* timer);

#include "threads.h"

#ifdef __cplusplus
}
#endif