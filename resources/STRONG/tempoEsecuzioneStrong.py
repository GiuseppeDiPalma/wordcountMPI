# importing package
import matplotlib.pyplot as plt
import numpy as np
  
# create data
x = [1,2,3,4,5,6,7,8]
y = [
    34.20851675,
    16.5369978,
    12.18462742,
    10.227383,
    8.629033833,
    7.59510805,
    6.843158167,
    6.746053317
]


t1 = 31.37578873


z = [
    t1/1,
    t1/2,
    t1/3,
    t1/4,
    t1/5,
    t1/6,
    t1/7,
    t1/8
]

# plot lines
plt.plot(x, y, label = "Strong Scaling")
plt.plot(x, z, label = "Best result")

plt.legend()
plt.grid(color='gray', ls = 'dotted', lw = 1.00)

plt.xlim([1, 8])
plt.axis
plt.title("Tempo esecuzione Strong Scalability")
plt.ylabel("Tempo(Secondi)")
plt.xlabel("Num CPU")

plt.savefig('timeSTRONG.png', format='png')