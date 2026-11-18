
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = bluffmaster

all:
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
