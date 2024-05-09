import matplotlib.pyplot as plt
import os
import sys

from netgraph import Graph

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

## setting for c5
## 6, 7
## (20, 25)

fig, axs = plt.subplots(
    2, 7, sharex=True, sharey=True, layout="tight", figsize=(16, 13)
)

k = 0
for i in range(2):
    for j in range(7):
        G = []
        T = []
        x = []
        y = []
        node_positions = {}
        with open(f"../.arcs{k}") as infile:
            num_of_points = int(infile.readline())
            for ii in range(num_of_points):
                line = infile.readline()
                xx, yy = line.strip().split(",")
                xx, yy = (float(xx), float(yy))
                node_positions[ii + 1] = (xx, yy)
                x.append(xx)
                y.append(yy)
            for line in infile:
                u, v = line.strip().split(",")
                e = (int(u), int(v))
                G.append(e)
                T.append(e)
            k = k + 1
        axs[i, j].plot(
            x,
            y,
            linestyle="--",
            alpha=0.3,
        )
        P = Graph(
            G,
            ax=axs[i, j],
            node_layout=node_positions,
            node_size=6,
            edge_width=2.2,
            edge_layout="arc",
        )

plt.savefig("arcs.svg", bbox_inches="tight")
plt.show()
