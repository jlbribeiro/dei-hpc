#!/usr/bin/env python

# a bar plot with errorbars
import numpy as np
import matplotlib.pyplot as plt

N = 4
tpsMeans = (56367.4793333, 93047.63, 107956.308333, 124385.612)
tpsStd =   (1588.9516329, 3863.58629287, 2635.64138111, 2757.97357834)

ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig = plt.figure()
ax = fig.add_subplot(111)
rects = ax.bar(ind, tpsMeans, width, color='r', yerr=tpsStd)

# add some
ax.set_xlabel('Number of cores')
ax.set_ylabel('Transactions per second')
ax.set_title('Transactions per second by number of cores')
ax.set_xticks(ind + width / 2)
ax.set_xticklabels( ('1 core', '2 cores', '3 cores', '4 cores') )

plt.show()