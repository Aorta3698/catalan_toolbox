import matplotlib.pyplot as plt
import os
import sys

from collections import defaultdict

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

n = 0
G = []
node_positions = {}
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
    for _ in range(int(infile.readline())):
        v, x, y = infile.readline().strip().split(",")
        node_positions[int(v)] = (float(x), float(y))
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

ax = plt.gca()
for e in G:
    (x1, y1), (x2, y2) = node_positions[e[0]], node_positions[e[1]]
    color, alpha = "black", 0.5
    ## dotted edges for mutze pattern avoiding trees
    if e in DE or (e[1], e[0]) in DE:
        color, alpha = "pink", 0.35
    ## full k-ary tree or non-full
    if not is_full and e in edges_del:
        alpha = 0
    ax.plot([x1, x2], [y1, y2], color=color, alpha=alpha, linewidth=2.4)

for v, (x, y) in node_positions.items():
    if is_full or v not in leaves_del:
        ax.plot(
            x,
            y,
            "o",
            markersize=14,
            markerfacecolor="white",
            markeredgecolor="black",
            markeredgewidth=1.2,
        )
ax.axis("off")

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
