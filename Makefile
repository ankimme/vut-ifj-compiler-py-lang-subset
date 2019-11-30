# Makefile
# Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
# VUT FIT

PROJECT=ifj19
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
LFLAGS = -lm
DFLAGS = -g -O0 # debug flags
FILES = main.c scanner.c dynamic_string.c stack.c symtable.c parser.c expressions.c
TEST_FOLDER = test
TEST_FILES = symtable_test scanner_test dynamic_string_test

.PHONY: all valgrind doc clean symtable_test scanner_test dynamic_string_test

$(PROJECT): $(FILES)
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $(FILES) $(LFLAGS)

all: $(PROJECT)

valgrind: $(PROJECT)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(PROJECT) < $(TEST_FOLDER)/test_input_1.py

doc:
	doxygen Doxyfile

clean:
	rm -f *.out *.o $(PROJECT) $(TEST_FILES)

symtable_test: symtable.c $(TEST_FOLDER)/symtable_test.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ symtable.c $(TEST_FOLDER)/$@.c $(LFLAGS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$@

scanner_test: scanner.c $(TEST_FOLDER)/scanner_test.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ scanner.c $(TEST_FOLDER)/$@.c stack.c dynamic_string.c $(LFLAGS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$@ < $(TEST_FOLDER)/scanner_input_1.txt

dynamic_string_test: dynamic_string.c $(TEST_FOLDER)/dynamic_string_test.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ dynamic_string.c $(TEST_FOLDER)/$@.c $(LFLAGS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$@