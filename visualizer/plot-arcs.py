import matplotlib.pyplot as plt
import os
import sys
from matplotlib.patches import Arc

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

ax = plt.gca()
for u, v in T:
    (x1, y1), (x2, _) = node_positions[u], node_positions[v]
    d = abs(x2 - x1)
    ax.add_patch(
        Arc(((x1 + x2) / 2, y1), d, d, theta1=180, theta2=360, color="pink", linewidth=2.7)
    )

ax.plot(
    x,
    y,
    "o",
    markersize=8,
    markerfacecolor="white",
    markeredgecolor="black",
    markeredgewidth=0.75,
)
ax.set_aspect("equal")
ax.axis("off")

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
