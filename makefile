BUILD_DIR = ./build/
TARGET = $(BUILD_DIR)6809_core
LIBS = cmocka
CC = gcc
CFLAGS = -I. -Wall -g
LWASM = lwasm
LWASMFLAGS = --6809 --format=decb

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard *.c)) \
	$(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard tests/**/*.c tests/*.c))
HEADERS = $(wildcard *.h) $(wildcard tests/*.h)
DECB_OBJECTS = $(patsubst %.s, $(BUILD_DIR)%.bin, $(wildcard tests/program/*.s))

$(BUILD_DIR)%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)%.bin: %.s
	$(LWASM) $(LWASMFLAGS) --output=$@ $<

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -l $(LIBS) -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f $(DECB_OBJECTS)

test: all $(DECB_OBJECTS)
	$(TARGET)
