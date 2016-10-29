BUILD_DIR = ./build/
TARGET = $(BUILD_DIR)6809_core
LIBS = cmocka
CC = gcc
CFLAGS = -I. -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

$(BUILD_DIR)%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -l $(LIBS) -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

test: all
	$(TARGET)
