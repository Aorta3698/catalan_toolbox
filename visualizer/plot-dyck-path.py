import matplotlib.pyplot as plt
import numpy as np
import os
import screen
import sys

from matplotlib.ticker import AutoMinorLocator, MultipleLocator

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"


def draw(ax, file):
    G = []
    x = []
    y = []
    node_positions = {}
    scale = 0
    with open(file) as infile:

        ## read info
        scale = float(infile.readline())
        num_of_points = int(infile.readline())

        ## set image dimension
        width = 4.8 + 0.30 * num_of_points
        height = 3.2 + 0.30 * num_of_points

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
    ax.set_xlim(auto=True)
    ## set minor (https://stackoverflow.com/questions/24943991/change-grid-interval-and-specify-tick-labels)
    ax.xaxis.set_minor_locator(MultipleLocator(scale))
    ax.yaxis.set_minor_locator(MultipleLocator(scale))
    ax.grid(which="minor", color="#CCCCCC", linestyle="--")
    ## turn off ticks https://stackoverflow.com/questions/12998430/how-to-remove-xticks-from-a-plot
    ax.xaxis.set_ticks_position("none")
    ax.yaxis.set_ticks_position("none")

    for u, v in G:
        (x1, y1), (x2, y2) = node_positions[u], node_positions[v]
        ax.plot([x1, x2], [y1, y2], color="black", alpha=0.5, linewidth=1.5)
    xs = [x for x, _ in node_positions.values()]
    ys = [y for _, y in node_positions.values()]
    ax.plot(
        xs,
        ys,
        "o",
        markersize=8,
        markerfacecolor="white",
        markeredgecolor="black",
        markeredgewidth=0.75,
    )

    ## same box for every path
    top = sum(max(y2 - y1, 0) for y1, y2 in zip(ys, ys[1:]))
    ax.set_ylim(-scale / 2, top + scale / 2)
    ax.set_xticks([])
    ax.set_yticks([])
    ax.set_aspect("equal")
    for spine in ax.spines.values():
        spine.set_visible(False)
    return width, height


if __name__ == "__main__":
    width, height = draw(plt.gca(), sys.argv[1])
    plt.gcf().set_size_inches(width, height)

    if len(sys.argv) == 2:
        screen.fit(plt.gcf())
        plt.show()
    else:
        path = "./out"
        if not os.path.exists(path):
            os.makedirs(path)
        plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
