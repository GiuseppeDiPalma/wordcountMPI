# importing package
import matplotlib.pyplot as plt
import numpy as np
  
# create data
x = [1,2,3,4,5,6,7,8]
y = [
    0.666246658,
    1.24509632,
    1.990213876,
    2.891925501,
    3.701555509,
    4.433496848,
    5.031130981,
    5.508056534,
]



t1 = 31.37578873


z = [
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
]

# plot lines
plt.plot(x, y, label = "Strong Scaling speedup")
plt.plot(x, z, label = "Best result possible")

plt.legend()
plt.grid(color='gray', ls = 'dotted', lw = 1.00)

plt.xlim([1, 8])
plt.axis
plt.title("SpeedUP Strong Scalability")
plt.ylabel("SpeedUP")
plt.xlabel("Num CPU")

plt.savefig('speedUpStrong.png', format='png')