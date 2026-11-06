import matplotlib.pyplot as plt
import numpy as np
import os
import sys

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

DE = set()
skip = 2

with open(sys.argv[1]) as infile:
    tmp = infile.readline().strip()
    pattern = tmp == "e"
    if pattern:
        n_dotted = int(infile.readline().strip())
        for _ in range(n_dotted):
            u, v = infile.readline().strip().split(",")
            u, v = int(u), int(v)
            DE.add((u, v))
        tmp = infile.readline().strip()
        skip += 2 + n_dotted

    is_full = tmp == "1"
    n = int(infile.readline())

nodes = np.loadtxt(sys.argv[1], delimiter=",", skiprows=skip, max_rows=n, ndmin=2)
edges = np.loadtxt(sys.argv[1], delimiter=",", skiprows=skip + n, dtype=int, ndmin=2)
ids = nodes[:, 0].astype(int)
x = np.zeros(ids.max() + 1)
y = np.zeros(ids.max() + 1)
x[ids] = nodes[:, 1]
y[ids] = nodes[:, 2]
u, v = edges[:, 0], edges[:, 1]


leaf = np.bincount(edges.ravel(), minlength=len(x)) == 1
shown = np.full(len(edges), is_full) | ~(leaf[u] | leaf[v])

dotted = np.full(len(edges), False)
for a, b in DE:
    dotted |= ((u == a) & (v == b)) | ((u == b) & (v == a))

leaves = x.max() + 1
levels = 1 - y.min()
width = min(14, max(6.4, 0.25 * leaves))
height = min(8, max(4.8, 0.45 * levels))
space = min(0.8 * 72 * width / leaves, 0.8 * 72 * height / levels)


def segments(keep):
    gap = np.full(keep.sum(), np.nan)
    xs = np.column_stack([x[u[keep]], x[v[keep]], gap]).ravel()
    ys = np.column_stack([y[u[keep]], y[v[keep]], gap]).ravel()
    return xs, ys


plt.figure(figsize=(width, height))
ax = plt.gca()
linewidth = min(2.4, max(0.3, space / 6))
xs, ys = segments(shown & ~dotted)
ax.plot(xs, ys, color="black", alpha=0.5, linewidth=linewidth)
xs, ys = segments(shown & dotted)
ax.plot(xs, ys, color="black", alpha=0.5, linewidth=linewidth, linestyle=":")

if space >= 4:
    drawn = ids if is_full else ids[~leaf[ids]]
    ax.plot(
        x[drawn],
        y[drawn],
        "o",
        markersize=22 if pattern else min(14, 0.7 * space),
        markerfacecolor="white",
        markeredgecolor="black",
        markeredgewidth=min(1.2, space / 10),
    )
    if is_full:
        inner = ids[~leaf[ids]]
        ax.plot(x[inner], y[inner], "o", markersize=min(14, 0.7 * space), color="black")
    if pattern:
        for v in drawn:
            ax.text(x[v], y[v], v, fontsize=12, ha="center", va="center_baseline")
ax.axis("off")

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
