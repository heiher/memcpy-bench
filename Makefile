CC=mips64el-unknown-linux-gnu-gcc
CFLAGS=-march=mips64r2 -mabi=64 -O2 -nostdlib -T ld.script

SOURCES=start.S memcpy.S main.c uart.c string.c

memcpy-bench : $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm -f memcpy-bench
