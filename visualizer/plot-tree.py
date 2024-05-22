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
DE = []
leaves_del = []
edges_del = []
is_full = 0

with open(sys.argv[1]) as infile:
    tmp = infile.readline().strip()
    if tmp == "e":
        n_dotted = int(infile.readline().strip())
        for _ in range(n_dotted):
            u, v = infile.readline().strip().split(",")
            u, v = int(u), int(v)
            DE.append((u, v))
        tmp = infile.readline().strip()

    is_full = tmp == "1"
    for line in infile:
        u, v = line.strip().split(",")
        u, v = int(u), int(v)
        G.append((u, v))  ## must be from parent to child (from my trial and error)
        E[u].append((u, v))
        E[v].append((u, v))

for key, val in E.items():
    if len(val) == 1:
        leaves_del.append(key)
        edges_del.append(val[0])

plot_instance = Graph(G, node_layout="dot")

## dotted edges for mutze pattern avoiding trees
for e in DE:
    if e not in plot_instance.edge_artists:
        ee = (e[1], e[0])
        e = ee
    plot_instance.edge_artists[e].set_color("pink")
    plot_instance.edge_artists[e].set_alpha(0.35)

## full k-ary tree or non-full
if not is_full:
    for e in edges_del:
        plot_instance.edge_artists[e].set_alpha(0)
    for v in leaves_del:
        plot_instance.node_artists[v].set_alpha(0)

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
