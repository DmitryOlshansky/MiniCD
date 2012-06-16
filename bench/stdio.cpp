#ifdef MINICD
#include "minicd.h"

#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t b_stdio_putcgetc(void *dummy)
{
	FILE *f = tmpfile();
	size_t i;
	size_t cs = 0;

	for (i=0; i<5000000; i++)
		putc('x', f);
	fseek(f, 0, SEEK_SET);
	for (i=0; i<5000000; i++)
		cs += getc(f);
	fclose(f);

	return cs;
}

size_t b_stdio_putcgetc_unlocked(void *dummy)
{
	FILE *f = tmpfile();
	size_t i;
	size_t cs = 0;

	for (i=0; i<5000000; i++)
#ifdef _MSC_VER 
		_putc_nolock('x', f);
#else 
		putc_unlocked('x', f);
#endif
	fseek(f, 0, SEEK_SET);
	for (i=0; i<5000000; i++)
#ifdef _MSC_VER
		cs += _getc_nolock(f);
#else 
		cs += getc_unlocked(f);
#endif
	fclose(f);

	return cs;
}

#endif