CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
TARGET := hnm
SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(SRC_FILES:.c=.o)

.PHONY := all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c hnm.h
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm -f *.o $(TARGET)