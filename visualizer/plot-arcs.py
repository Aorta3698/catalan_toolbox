import matplotlib.pyplot as plt
import os
import sys
import networkx as nx
from netgraph import Graph

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

G = []
T = []
node_positions = {}
x = []
y = []

with open(sys.argv[1]) as infile:
    num_of_points = int(infile.readline())
    for i in range(num_of_points):
        line = infile.readline()
        xx, yy = line.strip().split(",")
        xx, yy = float(xx), float(yy)
        node_positions[i + 1] = (xx, yy)
        x.append(xx)
        y.append(yy)
    for line in infile:
        u, v = line.strip().split(",")
        e = (int(u), int(v))
        G.append(e)
        T.append(e)

edge_labels = {}
id = 1
for e in T:
    edge_labels[e] = id
    id = id + 1


## set image dimension
width = 4.8 + 0.15 * num_of_points
height = 3.2 + 0.15 * num_of_points
plt.figure(figsize=(width, height))

## add line outline
plt.plot(
    x,
    y,
    linestyle="--",
    alpha=0.3,
)

## plot chords graph
P = Graph(
    G,
    node_layout=node_positions,
    edge_labels=edge_labels,
    edge_label_fontdict=dict(size=14),
    edge_layout="arc",
    # ax=ax,
)

## style inner edges
for e in T:
    P.edge_artists[e].set_color("pink")
    P.edge_artists[e].set_alpha(1.0)
    P.edge_artists[e].update_width(0.02)

plt.show()
