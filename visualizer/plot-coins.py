import matplotlib.pyplot as plt
import os
import sys

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

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
        k = k + 1

## set image dimension
width = 4.8 + 0.40 * num_of_coins_at_base
height = 3.2 + 0.40 * num_of_coins_at_base
plt.figure(figsize=(width, height))

ax = plt.gca()
for xx, yy in node_positions.values():
    ax.add_patch(
        plt.Circle((xx, yy), rad, facecolor="white", edgecolor="black", linewidth=0.75)
    )
ax.autoscale_view()
ax.set_aspect("equal")
ax.axis("off")

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
