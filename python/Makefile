LIBIUS=../library
IUSINCLUDES=-I$(LIBIUS)/src -I$(LIBIUS)/include -I$(LIBIUS)
IUSLIBS=-L../build/Darwin/library -lius-3.e23f52d4d

HDF5INCLUDES=-I/usr/local/Cellar/hdf5@1.8/1.8.20/include
HDF5LIBS=-L/usr/local/Cellar/hdf5@1.8/1.8.20/lib -lhdf5 -lhdf5_hl
PYTHONINCLUDES=-I/usr/local/Cellar/python/3.6.4_4/Frameworks/Python.framework/Versions/3.6/include/python3.6m
PYTHONLIBS=-L/usr/local/opt/python/Frameworks/Python.framework/Versions/3.6/lib/python3.6/config-3.6m-darwin -lpython3.6m -ldl -framework CoreFoundation

INCLUDES=$(IUSINCLUDES) $(PYTHONINCLUDES) $(HDF5INCLUDES) -Iinclude -I.
CFLAGS=$(INCLUDES)

test: test.py
	python $<

ius_wrap.c: ius.i
	swig -python $(INCLUDES) $<

_ius.so: ius_wrap.o
	ld -dylib $< -o $@ $(PYTHONLIBS) $(IUSLIBS) $(HDF5LIBS)

test.py: _ius.so
	echo "import ius" > test.py
	echo 'print(dir(ius))' >> test.py
	echo 'a = ius.IusAcquisition(1234.5,12122012,"Yesssss")' >> test.py
	echo 't = ius.IusHistoryNode("My Node Type")' >> test.py
	echo 'print(a)' >> test.py
	echo 'print(t)' >> test.py

clean:
	rm -f *.py
	rm -f *.so
	rm -f *.o
	rm -f ius_*

