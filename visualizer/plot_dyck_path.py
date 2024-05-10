import matplotlib.pyplot as plt
import numpy as np
import os
import sys

from netgraph import Graph
from matplotlib.ticker import AutoMinorLocator, MultipleLocator

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

G = []
x = []
y = []
node_positions = {}
scale = 0
with open(sys.argv[1]) as infile:

    ## read info
    scale = float(infile.readline())
    num_of_points = int(infile.readline())

    ## set image dimension
    width = 4.8 + 0.30 * num_of_points
    height = 3.2 + 0.30 * num_of_points
    plt.figure(figsize=(width, height))

    ## read point coordinates
    for i in range(num_of_points):
        line = infile.readline()
        xx, yy = line.strip().split(",")
        xx, yy = float(xx), float(yy)
        node_positions[i] = (xx, yy)

    ## read edges
    for line in infile:
        u, v = line.strip().split(",")
        e = (int(u), int(v))
        G.append(e)

## grid
ax = plt.subplot()
ax.set_xlim(auto=True)
ax.set_ylim(auto=True)
## set minor (https://stackoverflow.com/questions/24943991/change-grid-interval-and-specify-tick-labels)
ax.xaxis.set_minor_locator(MultipleLocator(scale))
ax.yaxis.set_minor_locator(MultipleLocator(scale))
ax.grid(which="minor", color="#CCCCCC", linestyle="--")
## turn off ticks https://stackoverflow.com/questions/12998430/how-to-remove-xticks-from-a-plot
ax.xaxis.set_ticks_position("none")
ax.yaxis.set_ticks_position("none")


Graph(
    G,
    node_layout=node_positions,
    ax=ax,
)

## none of the below works...

# ax.get_xaxis().set_ticks([])
# ax.get_yaxis().set_ticks([])
# ax.yaxis.set_ticks_position("left")
# ax.xaxis.set_ticks_position("bottom")
# ax.spines["top"].set_visible(False)
# ax.spines["right"].set_visible(False)
# ax.spines["bottom"].set_visible(False)
# ax.spines["left"].set_visible(False)
# ax.minorticks_off()
# plt.tick_params(
#     axis="y",  # changes apply to the x-axis
#     which="both",  # both major and minor ticks are affected
#     bottom=False,  # ticks along the bottom edge are off
#     top=False,  # ticks along the top edge are off
#     labelbottom=False,
# )  # labels along the bottom edge are off
# plt.tick_params(
#     axis="y",  # changes apply to the x-axis
#     which="both",  # both major and minor ticks are affected
#     bottom=False,  # ticks along the bottom edge are off
#     top=False,  # ticks along the top edge are off
#     labelbottom=False,
# )  # labels along the bottom edge are off

plt.show()
