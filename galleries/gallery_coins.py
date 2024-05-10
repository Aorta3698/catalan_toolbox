import matplotlib.pyplot as plt
import os
import sys

from netgraph import Graph

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

fig, axs = plt.subplots(
    2, 7, sharex=True, sharey=True, layout="tight", figsize=(16, 13)
)

n = 0
k = 0
for i in range(2):
    for j in range(7):
        node_positions = {}
        G = []
        n = 0
        with open(f"../.coins{k}") as infile:
            ## read needed info
            num_of_coins_at_base = int(infile.readline())
            rad = float(infile.readline())

            for line in infile:
                xx, yy = line.strip().split(",")
                xx, yy = float(xx), float(yy)
                node_positions[n] = (xx, yy)
                G.append((n, n))
                n = n + 1
        Graph(
            G,
            node_layout=node_positions,
            edge_alpha=0,
            node_size=rad * 100,
            node_edge_width=1.5,
            ax=axs[i, j],
        )
        k = k + 1

plt.savefig("coins.svg", bbox_inches="tight")
plt.show()
