# compiler variable declarations
CC = gcc
FLAGS=-Wall -pedantic
INC=-Isrc/
CFLAGS=$(FLAGS) -c -h --std=c99
LFLAGS=$(FLAGS) -lncurses
DIR_GUARD=@mkdir -p $(@D) #ensures output dirs for *.o and executables are created before attempting to build

# sources and objects
SOURCES=$(shell find src/ -type f -name "*.c")
OBJECTS=$(patsubst src/%.c, obj/%.o, $(SOURCES))
DEPS=$(patsubst src/%.c, deps/%.d, $(SOURCES))

# main targets
.PHONY: all clean clean_all

all: bin/main

GTAGS: $(SOURCES)
	gtags

clean:
	rm -rf obj/* bin/* src/*.gch GTAGS GPATH GRTAGS

clean_all:
	rm -rf bin/* obj/* deps/*

# --- compile Rule
obj/%.o: src/%.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $< -o $@

# --- link rule
bin/main: $(OBJECTS)
	$(DIR_GUARD)
	$(CC) $(OBJECTS) $(LFLAGS) -o bin/main

# --- dependency rule
deps/%.d: src/%.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -MM $< | sed -e 's/~\(.*\)\.o:/\1.d \1.o:/' > $@

ifneq "$(MAKECMDGOALS)" "clean_all"
	-include $(DEPS)
endif
