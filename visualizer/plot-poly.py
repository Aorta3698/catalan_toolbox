import matplotlib.pyplot as plt
import os
import sys
from netgraph import Graph

os.environ["XDG_SESSION_TYPE"] = "xcb"

G = []
node_positions = {}

## TODO: why does when num_of_points = 4, it just plots a straight line?
## I think it is a bug in Netgraph... oh well.
with open(sys.argv[1]) as infile:
    num_of_points = int(infile.readline())
    for i in range(num_of_points):
        line=infile.readline()
        x, y = line.strip().split(',')
        node_positions[i]={float(x),float(y)}
    for line in infile:
        u, v = line.strip().split(',')
        G.append((int(u), int(v)))

Graph(G, node_layout=node_positions)
plt.show()
