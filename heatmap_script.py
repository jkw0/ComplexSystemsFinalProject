import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap

path_to_csv = ""
df = np.loadtxt(path_to_csv)

fig, ax = plt.subplots()


colors = [(1, 1, 178/255), (254/255, 204/255, 92/255), (253/255, 141/255, 60/255), (240/255, 59/255, 32/255)]

cmap1 = LinearSegmentedColormap.from_list('mylist',colors, N=50)

plt.imshow(df, cmap=cmap1, interpolation='nearest', vmin = 0, vmax = 1)
plt.colorbar()

#plt.grid(True)

ax.set_xticks((0, 5, 10, 15, 20))
ax.set_xticklabels(("0.30", "0.35",
                    "0.40", "0.45", "0.50"))

ax.set_yticks((0, 4, 8, 12, 16, 20))
ax.set_yticklabels(("1", "0.8",
                    "0.6", "0.4", "0.2", "0.0"))


plt.minorticks_on()
plt.grid(b=True,which='minor', color='#FFFFFF', linestyle='-', alpha=0.6)