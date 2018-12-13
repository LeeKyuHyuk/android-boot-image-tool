CC			?= gcc
CFLAGS	+= -O2 -Wall -Werror -Wno-address-of-packed-member -I./include
LDFLAGS	+= -static

.PHONY: all clean

all:
	$(CC) $(CFLAGS) $(LDFLAGS) src/unpackbootimg.c -o unpackbootimg

clean:
	rm -fv unpackbootimg
