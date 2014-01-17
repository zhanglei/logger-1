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
