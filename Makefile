CC = gcc
CFLAGS = -std=c99 -Wall

all: A3_q1 

A3_q1: A3_q1.c
	$(CC) $(CFLAGS) A3_q1.c -o A3_q1

clean:
	rm -f A3_q1 