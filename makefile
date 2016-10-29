TARGET = 6809_core
LIBS = cmocka
CC = gcc
CFLAGS = -I. -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -l $(LIBS) -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
