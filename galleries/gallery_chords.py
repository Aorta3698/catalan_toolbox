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
        T = []
        node_positions = {}
        with open(f"../.chords{k}") as infile:
            num_of_points = int(infile.readline())
            for ii in range(num_of_points):
                line = infile.readline()
                x, y = line.strip().split(",")
                node_positions[ii + 1] = (float(x), float(y))
            for line in infile:
                u, v = line.strip().split(",")
                e = (int(u), int(v))
                G.append(e)
                T.append(e)
            k = k + 1
        P = Graph(
            G,
            ax=axs[i, j],
            node_layout=node_positions,
        )
        T = T[num_of_points::]
        for e in T:
            P.edge_artists[e].set_color("pink")
            P.edge_artists[e].set_alpha(1.0)
            P.edge_artists[e].update_width(0.02)

plt.savefig("chords.svg", bbox_inches="tight")
plt.show()
