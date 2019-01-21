app=/cadappl/matlab/R2017b/bin/mex

$app -g -largeArrayDims -I/home/310040622/IUS/libius/library/include -I/home/310040622/IUS/libius/library -output ../m/mxInputFile mxInputFile.c /home/310040622/IUS/libius/build/Linux/library/libius-3.1.0d.a /cadappl/python/2.7-64/lib/libhdf5.so /cadappl/python/2.7-64/lib/libhdf5_hl.so /cadappl/python/2.7-64/lib/libz.so -lgcov -luuid