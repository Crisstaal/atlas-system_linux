# Makefile for ELF Header Reader
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
LDFLAGS = -lelf 
TARGET = 0-hreadelf
SRC = 0-hreadelf.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
