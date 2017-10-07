SOURCES=src/unrolled_list.c
OBJECTS=$(SOURCES:.c=.o)
MAIN=main.c

LIB_NAME=libbirb

CFLAGS=
LDFLAGS=

obj/%.o: src/%.c
	clang $(CFLAGS) -c -o $@ $<

libbirb: $(OBJECTS)
	clang -shared -fPIC -o $(LIB_NAME).so $^

run: $(OBJECTS)
	clang $(MAIN) $^
	./a.out

.PHONY: run