import matplotlib.pyplot as plt
import os
import sys
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
# for e in T[num_of_points:]:
for e in T:
    edge_labels[e] = id
    id = id + 1

width = 4.8 + 0.15 * num_of_points
height = 3.2 + 0.15 * num_of_points
plt.figure(figsize=(width, height))
P = Graph(
    G,
    node_layout=node_positions,
    edge_labels=edge_labels,
    edge_label_fontdict=dict(size=14),
    # scale=(width, height),
)

# ## style root
# root = T[num_of_points - 1]
# P.edge_artists[root].set_color("green")
# P.edge_artists[root].set_alpha(0.5)
# P.edge_artists[root].update_width(0.02)

## style inner edges
# T = T[num_of_points::]
for e in T:
    P.edge_artists[e].set_color("pink")
    P.edge_artists[e].set_alpha(1.0)
    P.edge_artists[e].update_width(0.02)

plt.show()
