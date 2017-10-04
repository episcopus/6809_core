BUILD_DIR = ./build/
PROGRAM_DIR = $(BUILD_DIR)tests/program
TARGET = $(BUILD_DIR)6809.a
TEST_TARGET = $(BUILD_DIR)6809_tests
TEST_LIBS = -l cmocka -l png
MON_TARGET = $(BUILD_DIR)mon
CC = gcc
CFLAGS = -I. -Wall -g -D TESTS
LWASM = lwasm
LWASMFLAGS = --6809 --format=decb

.PHONY: default all clean test

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

TEST_OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard tests/**/*.c tests/*.c)) $(TARGET)
TEST_HEADERS = $(wildcard tests/*.h)
DECB_OBJECTS = $(patsubst %.s, $(BUILD_DIR)%.bin, $(wildcard tests/program/*.s))

MON_OBJECTS = $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard monitor/*.c)) $(TARGET)

$(BUILD_DIR)%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)%.bin: %.s
	$(LWASM) $(LWASMFLAGS) --output=$@ $<

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(TEST_TARGET): $(TARGET) $(TEST_OBJECTS) $(DECB_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(CFLAGS) $(TEST_LIBS) -o $@

$(MON_TARGET) : $(TARGET) $(MON_OBJECTS)
	$(CC) $(MON_OBJECTS) $(TEST_LIBS) $(CFLAGS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	rm -f $(TEST_TARGET)
	rm -f $(TEST_OBJECTS)
	rm -f $(DECB_OBJECTS)
	rm -f $(MON_TARGET)
	rm -f $(MON_OBJECTS)

test: $(TEST_TARGET)
	$(TEST_TARGET)

monitor: $(MON_TARGET) $(DECB_OBJECTS)
ifndef PROGRAM
	@echo "Syntax: make monitor PROGRAM=<program name from build/tests/program/*.bin, w/o extension>\n"
	@echo "Pick from the following:"
	@ls $(realpath $(PROGRAM_DIR))/*.bin
else
	$(MON_TARGET) $(realpath $(PROGRAM_DIR))/$(PROGRAM).bin
endif
