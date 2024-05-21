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

with open(sys.argv[1]) as infile:
    num_of_points = int(infile.readline())
    for i in range(num_of_points):
        line = infile.readline()
        x, y = line.strip().split(",")
        node_positions[i + 1] = (float(x), float(y))
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

## add circle outline
ax = plt.subplot()
circle = plt.Circle((0.0, 0.0), 1, fill=False, linestyle="--", alpha=0.3)
ax.add_patch(circle)

## plot chords graph
P = Graph(
    G,
    node_layout=node_positions,
    edge_labels=edge_labels,
    edge_label_fontdict=dict(size=14),
    ax=ax,
)

## style inner edges
for e in T:
    P.edge_artists[e].set_color("pink")
    P.edge_artists[e].set_alpha(1.0)
    P.edge_artists[e].update_width(0.02)

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
