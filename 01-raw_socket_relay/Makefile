CC = gcc
CFLAGS = -Wall -g -O0
SRC = ${wildcard *.c}
OBJS = ${patsubst %.c,%,$(SRC)}
SHELL=/bin/bash

all:$(OBJS)

%:%.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) ,*.sw? $(OBJS)

ncat-tx:
	ncat -u4 localhost 12222

ncat-rx:
	ncat -u4 -l localhost 13333