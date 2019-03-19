function writeIufIqFile( filename, iufInput )
% writeIufIqfile  Writes a hdf5 iufIqFile based on a MatLab iufInput struct

clear all;
close all;
clc;

% Set python version
[version, executable, isLoaded] = pyversion;
if ~isLoaded
    pyversion 3.6
end

% Register python library wrapper to python search path (uses distribution
% i.s.o. local build!
if ispc
    libpath = fullfile( pwd, '..', 'build', 'Windows', 'dist', 'python36', 'Windows');
else
    libpath = fullfile( pwd, '..', 'build', 'Linux', 'dist', 'python36', 'Windows');
end
if count(py.sys.path, libpath) == 0
    insert(py.sys.path,int32(0),libpath);
end

% Get iuf functions directly into local namespace
import py.Python3Iuf.*

% Full call: py.Python3Iuf.iufInputFileCreate('myfile4.input')
h = iufInputFileCreate('myfile.input')
p = iufParameterDictCreate()
iufParameterDictSet(p, 'myKey', 'myValue')
iufParameterDictGetSize(p)
d = py.dict(pyargs('key', 1, 'key2', 2))

x.key1 = 'value1';
x.key2 = 'value2';
d2 = py.dict(x)
s = struct(d2)

p = iufParameterDictCreate()
iufParameterDictSet(p, 'myKey', 'myValue')
s = struct(p)





%end