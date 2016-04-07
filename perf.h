/* perf.h
 * Heiher <r@hev.cc>
 */

#ifndef __PERF_H__
#define __PERF_H__

static inline void
enable_perf (void)
{
	unsigned int tmp;

	asm volatile (
		"li %[tmp], 0x40000002 \n"
		"mtc0 %[tmp], $25, 0 \n"
		:[tmp]"=&r"(tmp)
	);
}

static inline void
disable_perf (void)
{
	asm volatile ("mtc0 $0, $25, 0 \n");
}

static inline unsigned long
get_perf (void)
{
	unsigned long count;

	asm volatile (
		"dmfc0 %[count], $25, 1 \n"
		:[count]"=r"(count)
	);

	return count;
}

static inline void
set_perf (unsigned long count)
{
	asm volatile (
		"dmtc0 %[count], $25, 1 \n"
		::[count]"r"(count)
	);
}

#endif /* __PERF_H__ */

