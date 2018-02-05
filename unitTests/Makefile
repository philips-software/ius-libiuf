CC = gcc

FLAGS = -c
CFLAGS = -std=c99 -Wall -Wextra -pedantic -lm -fPIC -Iunity -g -I../include -I../lib/hdf5/include
LDFLAGS = -L../lib/ -lius -lm
TARGET  = ./unitTests

SOURCES = $(wildcard *.c unity/*.c)
OBJ = $(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJ)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/../lib:${PWD}/../lib/hdf5/lib ;\
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)  $(TARGET)

valgrind: $(TARGET)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/../lib:${PWD}/../lib/hdf5/lib ;\
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes  $(TARGET)
