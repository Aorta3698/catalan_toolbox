import matplotlib.pyplot as plt
import os
import sys

from collections import defaultdict
from netgraph import Graph

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

n = 0
G = []
E = defaultdict(list)
leaves_del = []
edges_del = []

with open(sys.argv[1]) as infile:
    for line in infile:
        u, v = line.strip().split(",")
        u, v = int(u), int(v)
        G.append((u, v))
        E[u].append((u, v))
        E[v].append((u, v))

for key, val in E.items():
    if len(val) == 1:
        leaves_del.append(key)
        edges_del.append(val[0])

plot_instance = Graph(G, node_layout="dot")

for e in edges_del:
    plot_instance.edge_artists[e].set_alpha(0)
for v in leaves_del:
    plot_instance.node_artists[v].set_alpha(0)


plt.show()
