# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

# List all targets and their corresponding source files
TARGETS = todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5
SOURCES = $(TARGETS:%=%.c)
OBJECTS = $(SOURCES:.c=.o)

# Default target to build all executables
all: $(TARGETS)

# Pattern rule to compile each target
%: %.o
	$(CC) $(CFLAGS) -o $@ $<

# Pattern rule to compile each target
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove compiled executables
.PHONY: clean re
clean:
	rm -f $(TARGETS) $(OBJECTS)

# Rebuild all targets
re: clean all