#
#  Makefile
#
#  Copyright (c) 2014 Daniel Rocha. All rights reserved.
#

CC=gcc
RM=rm

SOURCES    = example.c logger.c
OBJECTS    = $(SOURCES:.c=.o)
EXECUTABLE = example

CFLAGS = -Wall -pedantic -std=iso9899:1999 -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wconversion
LIBS   = -pthread

build: $(SOURCES)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(CFLAGS) $(LIBS)

clean:
	$(RM) -f $(OBJECTS) $(EXECUTABLE)
