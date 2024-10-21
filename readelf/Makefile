# Makefile for ELF Header Reader
CC = gcc
CFLAGS = -Wall -g
TARGET = hreadelf

all: $(TARGET)

$(TARGET): 0-hreadelf.c
	$(CC) $(CFLAGS) -o $(TARGET) 0-hreadelf.c -lelf

clean:
	rm -f $(TARGET)
