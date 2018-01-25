CC = gcc
#MCS = mcs

FLAGS = -c
CFLAGS = -lm -fPIC -Iunity -g -I../include 
LDFLAGS = -L../lib/ -lius -lm

OBJTARGET = libalgo.so
TARGET  = unitTests.exe

SOURCES = $(wildcard *.c) unity/unity.c
OBJECTS = *.o

#CSSOURCES = .cs ../../Cs/NewClusterServer/Const.cs ../../Cs/NewClusterServer/IusServiceContract.cs ../../Cs/NewClusterServer/UriProvider.cs

all: clean $(TARGET)

clean:
	rm -f *.so *.o
	rm -f *.exe

$(TARGET):
	#$(MCS) -out:$(TARGET) -r:System.ServiceModel.dll -r:System.Runtime.Serialization -r:Mono.Posix.dll $(CSSOURCES)
	$(CC) $(FLAGS) $(CFLAGS) $(SOURCES)
	$(CC) $(LDFLAGS) -o $(OBJTARGET) $(OBJECTS)


