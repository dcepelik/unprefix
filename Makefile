CFLAGS += -Wall -Werror -Wextra -Wno-unused-function --pedantic -std=gnu11 -ggdb3

unprefix: unprefix.c Makefile
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f -- unprefix
