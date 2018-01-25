SHELL = /bin/sh
CC    = gcc

FLAGS        = # -std=gnu99 #-Iinclude
CFLAGS       = -Llib/hdf5/lib -lhdf5 -lhdf5_hl -lpthread -luuid -lm -Ilib/hdf5/include -Iinclude -fPIC -g #-pedantic -Wall -Wextra -ggdb3
LDFLAGS      = -shared

TARGET  = libius_v3.so
#SOURCES = $(wildcard src/*.c) $(wildcard src/base/*.c) $(wildcard src/fileio/*.c) 
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) lib/libius.so

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(OBJECTS) -o lib/$@ $(LDFLAGS)



