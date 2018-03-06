CC = gcc

FLAGS = -c
CFLAGS = -luuid -lm -Iinclude -g -I../../lib/hdf5/include -I../../include -g -L../../lib
LDFLAGS = -L../../lib/ -L../../lib/hdf5/lib -lius -lm -lhdf5 -lhdf5_hl -luuid


#-lius 

TARGET  = testVera2Input

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

all: $(TARGET)

clean:
	rm -f  *.o $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)


