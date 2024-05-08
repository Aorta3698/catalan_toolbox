import matplotlib.pyplot as plt

# import networkx as nx

# import pydot
import os
import sys

from netgraph import Graph

from networkx.drawing.nx_pydot import graphviz_layout

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

# G = nx.Graph()
G = []

with open(sys.argv[1]) as infile:
    for line in infile:
        u, v = line.strip().split(",")
        G.append((int(u), int(v)))
        # G.add_edge(u, v)

# pos = graphviz_layout(G, prog="dot")
# nx.draw(G, pos)
Graph(G, node_layout="dot")
plt.show()
