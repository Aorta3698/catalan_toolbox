import matplotlib.pyplot as plt
import networkx as nx
import pydot
import os
import sys
from networkx.drawing.nx_pydot import graphviz_layout

os.environ["XDG_SESSION_TYPE"] = "xcb"

G = nx.Graph()

with open(sys.argv[1]) as infile:
    for line in infile:
        u, v = line.strip().split(",")
        G.add_edge(u, v)

pos = graphviz_layout(G, prog="dot")
nx.draw(G, pos)
plt.show()
