import matplotlib.pyplot as plt
import os
import sys

from netgraph import Graph
from matplotlib.ticker import AutoMinorLocator, MultipleLocator

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
        node_positions = {}
        with open(f"../.dyck{k}") as infile:
            ## read info
            scale = float(infile.readline())
            num_of_points = int(infile.readline())

            ## read point coordinates
            for ii in range(num_of_points):
                line = infile.readline()
                xx, yy = line.strip().split(",")
                xx, yy = float(xx), float(yy)
                node_positions[ii] = (xx, yy)

            ## read edges
            for line in infile:
                u, v = line.strip().split(",")
                e = (int(u), int(v))
                G.append(e)

        ## grid
        axs[i, j].set_xlim(auto=True)
        axs[i, j].set_ylim(auto=True)
        ## set minor (https://stackoverflow.com/questions/24943991/change-grid-interval-and-specify-tick-labels)
        axs[i, j].xaxis.set_minor_locator(MultipleLocator(scale))
        axs[i, j].yaxis.set_minor_locator(MultipleLocator(scale))
        axs[i, j].grid(which="minor", color="#CCCCCC", linestyle="--")
        ## turn off ticks https://stackoverflow.com/questions/12998430/how-to-remove-xticks-from-a-plot
        axs[i, j].xaxis.set_ticks_position("none")
        axs[i, j].yaxis.set_ticks_position("none")

        Graph(
            G,
            node_layout=node_positions,
            ax=axs[i, j],
        )

        k = k + 1

plt.savefig("dyck_path.svg", bbox_inches="tight")
plt.show()
