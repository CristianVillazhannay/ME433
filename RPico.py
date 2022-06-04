from ulab import numpy as np
from ulab import scipy as spy
import math
import time


#Initalizing an empty array
t = np.arange(0,1,1/1024);
amat = np.zeros(1024);

for i in range(0,1023,1):
    y = 3*math.sin(3.14 * 3 * i)
    x = 8*math.sin(6.28 * 8 * i)
    z = math.sin(12.56 * 4 * i)
    amat[i] = y + x + z;

    #Plotting
    print((amat[i],))
    time.sleep





