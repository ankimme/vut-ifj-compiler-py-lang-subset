# Makefile
# Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
# VUT FIT

PROJECT=ifj19
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
LFLAGS = -lm
DFLAGS = -g -O0 # debug flags
FILES = main.c symtable.c scanner.c dynamic_string.c

.PHONY: all clean doc

$(PROJECT): $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES) $(LFLAGS)

all: $(PROJECT)

doc:
	doxygen Doxyfile

clean:
	rm -f *.out *.o $(PROJECT)