CC = gcc

FLAGS = -c
CFLAGS = -lm -fPIC -Iunity -g -I../include 
LDFLAGS = -L../lib/ -lius -lm
TARGET  = unitTests

SOURCES = $(wildcard *.c unity/*.c)
OBJ = $(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)  $(TARGET)

