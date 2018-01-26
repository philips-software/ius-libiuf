CC = gcc

FLAGS = -c
CFLAGS = -lm -fPIC -Iunity -g -I../include 
LDFLAGS = -L../lib/ -lius -lm

OBJTARGET = libalgo.so
TARGET  = unitTests

SOURCES = $(wildcard *.c) unity/unity.c
OBJECTS = *.o


all: clean $(TARGET)

clean:
	rm -f *.so *.o $(TARGET)

$(TARGET):
	$(CC) $(FLAGS) $(CFLAGS) $(SOURCES)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)


