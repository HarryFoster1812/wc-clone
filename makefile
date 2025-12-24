CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = wc-clone
BIN_DIR = bin
SRC = src/main.c

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	rm -rf $(BIN_DIR)
