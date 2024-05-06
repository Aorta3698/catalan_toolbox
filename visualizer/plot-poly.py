import matplotlib.pyplot as plt
import os
import sys
from netgraph import Graph

os.environ["XDG_SESSION_TYPE"] = "xcb"

G = []
T = []
node_positions = {}

## TODO: why does when num_of_points = 4, it just plots a straight line?
## I think it is a bug in Netgraph... oh well.
with open(sys.argv[1]) as infile:
    num_of_points = int(infile.readline())
    for i in range(num_of_points):
        line = infile.readline()
        x, y = line.strip().split(",")
        node_positions[i] = (float(x), float(y))
    for line in infile:
        u, v = line.strip().split(",")
        e = (int(u), int(v))
        G.append(e)
        T.append(e)

P = Graph(G, node_layout=node_positions)

## style root
root = T[num_of_points - 1]
P.edge_artists[root].set_color("green")
P.edge_artists[root].set_alpha(0.5)
P.edge_artists[root].update_width(0.02)

## style inner edges
T = T[num_of_points::]
for e in T:
    P.edge_artists[e].set_color("pink")
    P.edge_artists[e].set_alpha(1.0)
    P.edge_artists[e].update_width(0.02)

plt.show()
