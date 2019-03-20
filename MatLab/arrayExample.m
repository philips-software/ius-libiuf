clear all
close all
clc

myArray = zeros(5,4,3);
myArray(1,1,1) = 1;
myArray(2,1,1) = 2;
myArray(1,2,1) = 3;
myArray(2,2,1) = 4;
myArray(1,1,2) = 5;
myArray(2,1,2) = 6;
myArray(1,2,2) = 7;
myArray(2,2,2) = 8;

myArray
myVector = reshape(myArray, 1, 3*4*5)

x = py.numpy.array(myArray);
x.reshape(1,60)

%py.numpy.reshape(py.numpy.array(myArray), (1,60))

