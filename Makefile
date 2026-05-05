TARGET = game
CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -Ibuild/_deps/raylib-src/src -O2 -g -O0
LIBS = build/_deps/raylib-build/raylib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
OBJ_DIR = build/obj

# 1. Find ALL .c files in src and its subfolders
SRCS = $(shell find $(SRC_DIR) -name '*.c')

# 2. Convert src/path/to/file.c -> build/path/to/file.o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# 3. Compile and automatically create subdirectories in build/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)

.PHONY: all clean
