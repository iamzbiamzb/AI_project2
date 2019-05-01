import numpy as np
from sklearn import datasets

npd = np.genfromtxt('ellipse100.txt', delimiter='')

#print(npd.ndim, npd.shape, npd.dtype)

print(npd)