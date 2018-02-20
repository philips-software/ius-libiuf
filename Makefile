SHELL = /bin/sh
CC    = gcc

FLAGS        = # -std=gnu99 #-Iinclude
CFLAGS       = -std=c99 -Llib/hdf5/lib -lhdf5 -lhdf5_hl -lpthread -luuid -lm -Ilib/hdf5/include -Iinclude -fPIC -g -pedantic -Wall -Wextra # -ggdb3
#CFLAGS       = -lhdf5 -lhdf5_hl -lpthread -luuid -lm -I/usr/include/hdf5 -Iinclude -fPIC -g #-pedantic -Wall -Wextra -ggdb3
LDFLAGS      = -shared

VERSION = 3.0
VERSION_TARGET  = lib/libius.so.$(VERSION)
TARGET  = lib/libius.so

#SOURCES = $(wildcard src/*.c) $(wildcard src/base/*.c) $(wildcard src/fileio/*.c) 
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) 

$(VERSION_TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

$(TARGET): $(VERSION_TARGET)
	cp $(VERSION_TARGET) $(TARGET) 

