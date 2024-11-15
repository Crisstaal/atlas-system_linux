CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic
LDFLAGS := -lelf

TARGET := hnm
SRC_FILES := $(wildcard *.c)
OBJ_FILES := $(SRC_FILES:.c=.o)

.PHONY := all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm -f *.o $(TARGET)