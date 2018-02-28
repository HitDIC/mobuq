import struct
import matplotlib.pyplot as plt
import numpy as np

data = np.genfromtxt("seq.txt")

for i in range(4):
    for j in range(i+1, 4):
        fig = plt.figure()
        ax = fig.add_subplot(111)
        ax.scatter(data[:, i], data[:, j])
        ax.set_xlabel("%d" % i)
        ax.set_ylabel("%d" % j)
        ax.set_xlim([-600, -200])
        ax.set_ylim([-600, -200])
        plt.savefig("%d-%d.jpg" % (i, j))
        # plt.clear()