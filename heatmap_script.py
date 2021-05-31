import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap

path_to_csv = "output_Heat/KWA__M1_n1000_g50000_s3_IA0.0_pA0.3_IB1.0_pB0.7_selfInfltrue_pA_End0.50_pA_Step0.01_IA_End1.00_IA_Step0.10.txt"
df = np.loadtxt(path_to_csv)

arr = (df[0], df[0],
       df[1], df[1],
       df[2], df[2],
       df[3], df[3],
       df[4], df[4],
       df[5], df[5],
       df[6], df[6],
       df[7], df[7],
       df[8], df[8],
       df[9], df[9],
       df[10], df[10])
       

fig, ax = plt.subplots()


colors = [(1, 1, 178/255), (254/255, 204/255, 92/255), (253/255, 141/255, 60/255), (240/255, 59/255, 32/255)]

cmap1 = LinearSegmentedColormap.from_list('mylist',colors, N=50)

plt.imshow(arr, cmap=cmap1, interpolation='nearest', vmin = 0, vmax = 1)
plt.colorbar()

#plt.grid(True)

ax.set_xticks((0, 5, 10, 15, 20))
ax.set_xticklabels(("0.30", "0.35",
                    "0.40", "0.45", "0.50"))

ax.set_yticks((0, 4, 8, 12, 16, 20))
ax.set_yticklabels(("1", "0.8", "0.6", "0.4", "0.2", "0.0"))


plt.minorticks_on()
plt.grid(b=True,which='minor', color='#FFFFFF', linestyle='-', alpha=0.6)

plt.savefig("h2")
plt.show()