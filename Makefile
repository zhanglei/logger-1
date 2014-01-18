#
#  Makefile
#
#  Copyright (c) 2014 Daniel Rocha. All rights reserved.
#

CC=gcc
RM=rm
CFLAGS=-Wall -Wextra
SOURCES=example.c logger.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=example

build: $(SOURCES)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(CFLAGS)

clean:
	$(RM) -f $(OBJECTS) $(EXECUTABLE)
