import random

import matplotlib.patches as mpatches
import matplotlib.pyplot as plt

# import numpy as np


fig = plt.figure(figsize=(5, 5))
ax = plt.subplot(111, aspect=1)


art = mpatches.Rectangle((-1, -1), 2, 2, facecolor="white",
                         ec='b', alpha=1, lw=1)
ax.add_patch(art)

art = mpatches.Circle((0., 0.), 1., facecolor="c", ec="b", lw=2, alpha=0.25)
ax.add_patch(art)

art = mpatches.Circle((0., 0.), 0.5, facecolor="c", ec="g", lw=2, alpha=0.25)
ax.add_patch(art)


ax.set_xticks([])
ax.set_yticks([])
ax.axis([-1.1, 1.1, -1.1, 1.1])
ax.axis([0, 1.05, 0, 1.05])

nptos = 0
ptosInR1 = 0
ptosInR2 = 0
ptosInR3 = 0
nsim = 5000
# random.seed(10)
for i in range(0, nsim):
    # x = np.random.rand()
    # y = np.random.rand()
    x = random.random()
    y = random.random()
    nptos += 1
    if x**2 + y**2 <= 1:
        ptosInR1 += 1
        if x <= 0.6:
            ptosInR2 += 1
            if x**2 + y**2 > 0.5**2:
                ptosInR3 += 1
                ax.plot([x], [y], 'yx', lw=2, ms=5)
            else:
                ax.plot([x], [y], 'gx', lw=2, ms=5)
        else:
            ax.plot([x], [y], 'bx', lw=2, ms=5)
    else:
        ax.plot([x], [y], 'rx', lw=2, ms=5)

print('ptosInR1 = ', ptosInR1)
print('ptosInR2 = ', ptosInR2)
print('ptosInR3 = ', ptosInR3)
areaR1 = 4*float(ptosInR1)/float(nsim)
areaR2 = 4*float(ptosInR2)/float(nsim)
areaR3 = 4*float(ptosInR3)/float(nsim)
print('R1 =', areaR1)
print('R2 =', areaR2)
print('R3 =', areaR3)
s = 'Nsim =' + str(nsim) + '  R1 = ' + str(areaR1) \
    + '  R2 = ' + str(areaR2) + '  R3 = ' + str(areaR3)
plt.title(s)
plt.show()
