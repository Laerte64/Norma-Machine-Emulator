CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
TARGET = program

SRC_DIR = source
INCLUDE_DIR = include
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)