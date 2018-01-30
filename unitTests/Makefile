CC = gcc

CFLAGS = -Iunity -I../include -I./ -g -Wall
LDFLAGS = -L../lib/ -lius -lm

TARGET = main

SOURCES = $(wildcard *.c) unity/unity.c
OBJECTS = *.o


all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES)
	$(CC) $(LDFLAGS) -o $(OBJECTS) $(TARGET).c


