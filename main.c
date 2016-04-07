/* main.c
 * Heiher <r@hev.cc>
 */

#include "uart.h"
#include "perf.h"
#include "string.h"

#define SIZE	(1024 * 1024 + 16)

static char dst[SIZE] __attribute__((aligned(16)));
static char src[SIZE] __attribute__((aligned(16)));

static void __attribute__((optimize("O0")))
loop_bench (void *dst, const void *src, unsigned long len)
{
	unsigned int i;

	for (i=0; i<2048; i++) {
		memcpy (dst, src, len);
		memcpy (dst, src, len);
		memcpy (dst, src, len);
		memcpy (dst, src, len);
		memcpy (dst, src, len);
		memcpy (dst, src, len);
		memcpy (dst, src, len);
		memcpy (dst, src, len);
	}
}

static int
memcmp (const void *p1, const void *p2, unsigned long n)
{
	const unsigned char *s1 = p1, *s2 = p2;
	unsigned long i = 0;

	for (i=0; i<n; i++)
	      if (s1[i] != s2[i])
		    return s1[i] - s2[i];

	return 0;
}

static void
run_bench (void)
{
	int n, i, j;
	char buf[32];
	unsigned long cycles, sizes[] = { 1, 16, 64, 1024, 16384, SIZE - 16 };

	for (n=0; n<(sizeof(sizes)/sizeof(unsigned long)); n++) {
		for (i=0; i<16; i++) {
			for (j=0; j<16; j++) {
				memcpy (dst + i, src + j, sizes[n]);
				if (0 != memcmp (dst + i, src + j, sizes[n])) {
					puts ("FAIL\r\n");
					break;
				}
				disable_perf ();
				set_perf (0);
				enable_perf ();

				loop_bench (dst + i, src + j, sizes[n]);

				cycles = get_perf ();
				disable_perf ();

				puts ("dst: ");
				ultostr (buf, (unsigned long) dst + i);
				puts (buf);
				puts (" src: ");
				ultostr (buf, (unsigned long) src + j);
				puts (buf);
				puts (" len: ");
				ultostr (buf, (unsigned long) sizes[n]);
				puts (buf);
				puts (" cycles: ");
				ultostr (buf, cycles);
				puts (buf);
				puts ("\r\n");
			}
		}
	}
}

void
master_main (void)
{
	run_bench ();
	puts ("FINISH\r\n");
	asm volatile ("break":::"memory");
}

void
slave_main (unsigned int id)
{
}

