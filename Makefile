SOURCES=src/unrolled_list.c
OBJECTS=$(SOURCES:.c=.o)
MAIN=main.c

LIB_NAME=libbirb

CFLAGS=-g
LDFLAGS=

obj/%.o: src/%.c
	clang $(CFLAGS) -c -o $@ $<

build: $(OBJECTS)
	clang $(CFLAGS) -shared -fPIC -o $(LIB_NAME).so $^

run: $(OBJECTS)
	clang $(CFLAGS) $(MAIN) $^
	./a.out

.PHONY: run build