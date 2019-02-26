
import numpy as np

myArray = np.zeros((4,3,2))
myArray[0][0][0] = 1
myArray[0][0][1] = 2
myArray[0][1][0] = 3
myArray[0][1][1] = 4
myArray[1][0][0] = 5
myArray[1][0][1] = 6
myArray[1][1][0] = 7
myArray[1][1][1] = 8

myVector = myArray.reshape((1, 24))
myArray
myVector
