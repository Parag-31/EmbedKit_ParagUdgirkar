CC=gcc
CFLAGS=-Wall -std=c99

all:
	$(CC) $(CFLAGS) uart_parser.c -o uart_parser

run:
	./uart_parser

clean:
	rm -f uart_parser