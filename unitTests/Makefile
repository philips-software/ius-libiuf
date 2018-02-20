CC = gcc
FLAGS = -c
VERSION = 3.0

CPP_DEFINES = -DUNITY_INCLUDE_DOUBLE
CFLAGS = $(CPP_DEFINES) -std=c99 -Wall -Wextra -pedantic -lm -fPIC -Iunity -g -I../include -I../lib/hdf5/include
LDFLAGS = -L../lib/ -lius -lm
TARGET  = ./unitTests

SOURCES = $(wildcard *.c unity/*.c)
OBJ = $(SOURCES:.c=.o)
VALGRIND_EXIT_CODE=10

all: $(TARGET)

$(TARGET): $(OBJ)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/../lib:${PWD}/../lib/hdf5/lib ;\
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)  $(TARGET)

test: $(TARGET)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/../lib:${PWD}/../lib/hdf5/lib ;\
	$(TARGET)

valgrind: $(TARGET)
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/../lib:${PWD}/../lib/hdf5/lib ;\
	valgrind --error-exitcode=$(VALGRIND_EXIT_CODE) --tool=memcheck --leak-check=yes --track-origins=yes  $(TARGET)
