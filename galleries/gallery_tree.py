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
        G = []
        with open(f"../.tree{k}") as infile:
            for line in infile:
                u, v = line.strip().split(",")
                G.append((int(u), int(v)))
            k = k + 1
        Graph(G, ax=axs[i, j], node_layout="dot")

plt.savefig("tree.svg", bbox_inches="tight")
plt.show()
