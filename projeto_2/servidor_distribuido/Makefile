CC = gcc
# LDFLAGS = -lwiringPi
LDFLAGS = 
CFLAGS = -c -Wall -I $(INC_DIR) -I -pedantic

BUILD_DIR = .
INC_DIR  = $(BUILD_DIR)/inc
SRC_DIR  = $(BUILD_DIR)/src
OBJ_DIR  = $(BUILD_DIR)/obj
BIN_DIR  = $(BUILD_DIR)/bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
BIN = $(BIN_DIR)/bin

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ_DIR)/*.o -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf $(OBJ_DIR)/*  $(BIN_DIR)/*

run:
	@$(BIN)