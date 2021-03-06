CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address -o3
LDFLAGS = -fsanitize=address
SCR = main.c dictionary.c autocomplete.c correction.c
OBJ = $(SCR:.c=.o)

all: main

simple_editor: simple_editor.o
simple_editor.o: simple_editor.c

test: test.o
test.o: test.c

main: ${OBJ}
main.o: main.c dictionary.h autocomplete.h correction.h
dictionary.o: dictionary.c dictionary.h
autocomplete.o: autocomplete.c autocomplete.h dictionary.h
correction.o: correction.c correction.h dictionary.h

.PHONY: clean

clean:
	${RM} main test ${OBJ}
	${RM} *.o