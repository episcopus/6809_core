BUILD_DIR = ./build/
TARGET = $(BUILD_DIR)6809.a
TEST_TARGET = $(BUILD_DIR)6809_tests
TEST_LIBS = cmocka $(TARGET)
LIBS =
CC = gcc
CFLAGS = -I. -Wall -g
LWASM = lwasm
LWASMFLAGS = --6809 --format=decb

.PHONY: default all clean test

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

TEST_OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard tests/**/*.c tests/*.c))
TEST_HEADERS = $(wildcard tests/*.h)
DECB_OBJECTS = $(patsubst %.s, $(BUILD_DIR)%.bin, $(wildcard tests/program/*.s))

$(BUILD_DIR)%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)%.bin: %.s
	$(LWASM) $(LWASMFLAGS) --output=$@ $<

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(TEST_TARGET): $(TARGET) $(TEST_OBJECTS) $(DECB_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(CFLAGS) -l $(TEST_LIBS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	rm -f $(TEST_TARGET)
	rm -f $(TEST_OBJECTS)
	rm -f $(DECB_OBJECTS)

test: $(TEST_TARGET)
	$(TEST_TARGET)
