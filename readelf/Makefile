# Makefile for ELF Header Reader
CC = gcc
CFLAGS = -Wall -g
TARGET = 0-hreadelf

all: $(TARGET)

$(TARGET): 0-hreadelf.c hreadelf.h
	$(CC) $(CFLAGS) -o $(TARGET) 0-hreadelf.c -lelf

clean:
	rm -f $(TARGET)
