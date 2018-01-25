SHELL = /bin/sh
CC    = gcc

FLAGS        = # -std=gnu99 #-Iinclude
CFLAGS       = -Llib/hdf5/lib -lhdf5 -lhdf5_hl -lpthread -luuid -lm -Ilib/hdf5/include -Iinclude/V3 -fPIC -g #-pedantic -Wall -Wextra -ggdb3
LDFLAGS      = -shared

TARGET  = libius_v3.so
#SOURCES = $(wildcard src/*.c) $(wildcard src/base/*.c) $(wildcard src/fileio/*.c) 
SOURCES = $(wildcard src/V3/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) lib/libius_v3.so

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(OBJECTS) -o lib/$@ $(LDFLAGS)



