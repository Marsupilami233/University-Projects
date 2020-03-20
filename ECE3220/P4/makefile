# This make file is for Project4 only with a single file!
CC	= gcc
CFLAGS	= -Wall -g

paging:	paging.o
	$(CC) -o paging paging.o

.PHONY: test
test1: paging
	./paging -v < trace1.txt

test2: paging
	./paging -v < trace2.txt

test3: paging
	./paging -v < trace3.txt

.PHONY: clean
clean:
	rm paging.o paging
