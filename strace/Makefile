CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

# Targets and their source files
TARGETS = strace_0 strace_1
SOURCES = strace_0.c strace_1.c

all: $(TARGETS)

# Generic rule for building each target
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
