CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
LDFLAGS = -lelf 
TARGET = hnm
SRC = hnm.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
