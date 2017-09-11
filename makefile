BUILD_DIR = ./build/
TARGET = $(BUILD_DIR)6809_core
LIBS = cmocka
CC = gcc
CFLAGS = -I. -Wall -g

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard *.c)) \
	$(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard tests/*.c)) \
	$(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard tests/unit/*.c))
HEADERS = $(wildcard *.h) $(wildcard tests/*.h)

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
