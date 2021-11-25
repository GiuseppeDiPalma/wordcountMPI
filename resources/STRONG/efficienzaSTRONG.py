# importing package
import matplotlib.pyplot as plt
import numpy as np
  
# create data
x = [1,2,3,4,5,6,7,8]
y = [
    0.696246658,
    0.69954816,
    0.690071292,
    0.680481375,
    0.676311102,
    0.672249475,
    0.668732997,
    0.661007067
]




t1 = 31.37578873


z = [
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
]

# plot lines
plt.plot(x, y, label = "Strong Scaling speedup")
plt.plot(x, z, label = "Best result possible")

plt.legend()
plt.grid(color='gray', ls = 'dotted', lw = 1.00)

plt.xlim([1, 8])
plt.ylim([0.0, 1.4])
plt.axis
plt.title("Efficiency Strong Scalability")
plt.ylabel("Efficiency")
plt.xlabel("Num CPU")

plt.savefig('efficienzaStrong.png', format='png')