CC=gcc
CFLAGS=-c -Wall -Wextra -Werror
SOURCES=grep.c options.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=s21_grep
all: $(SOURCES) s21_grep
.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o $(OBJECTS) ../build/s21_grep

rebuild: clean all

s21_grep: $(OBJECTS)
	rm -rf grep.o options.o
	$(CC) $(CFLAGS) grep.c options.c 
	$(CC) $(OBJECTS) -o $(EXECUTABLE)
	rm -rf grep.o options.o
