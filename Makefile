CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude
SRC = src/main.c src/value.c src/table.c

.PHONY: all test clean

all: minidb.exe

minidb.exe: $(SRC) include/minidb.h
	$(CC) $(CFLAGS) $(SRC) -o minidb.exe

test_m1.exe: tests/test_m1.c src/value.c src/table.c include/minidb.h
	$(CC) $(CFLAGS) tests/test_m1.c src/value.c src/table.c -o test_m1.exe

test: test_m1.exe
	./test_m1.exe

clean:
	rm -f minidb.exe test_m1.exe
