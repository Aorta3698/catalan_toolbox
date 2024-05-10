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
k = 0

## add circle outline
num_of_coins_at_base = 0
rad = 0

with open(sys.argv[1]) as infile:

    ## read needed info
    num_of_coins_at_base = int(infile.readline())
    rad = float(infile.readline())

    for line in infile:
        xx, yy = line.strip().split(",")
        xx, yy = float(xx), float(yy)
        node_positions[k] = (xx, yy)
        G.append((k, k))
        k = k + 1

## set image dimension
width = 4.8 + 0.40 * num_of_coins_at_base
height = 3.2 + 0.40 * num_of_coins_at_base
plt.figure(figsize=(width, height))

Graph(
    G,
    node_layout=node_positions,
    edge_alpha=0,
    node_size=rad * 100,
    node_edge_width=1,
)
plt.show()
