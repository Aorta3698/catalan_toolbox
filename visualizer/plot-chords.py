import matplotlib.pyplot as plt
import os
import sys

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"


def draw(ax, file):
    G = []
    T = []
    node_positions = {}

    with open(file) as infile:
        num_of_points = int(infile.readline())
        for i in range(num_of_points):
            line = infile.readline()
            x, y = line.strip().split(",")
            node_positions[i] = (float(x), float(y))
        for line in infile:
            u, v = line.strip().split(",")
            e = (int(u), int(v))
            G.append(e)
            T.append(e)

    ## set image dimension
    width = 4.8 + 0.15 * num_of_points
    height = 3.2 + 0.15 * num_of_points

    ## add circle outline
    circle = plt.Circle((0.0, 0.0), 1, fill=False, linestyle="--", alpha=0.3)
    ax.add_patch(circle)

    for u, v in T:
        (x1, y1), (x2, y2) = node_positions[u], node_positions[v]
        ax.plot([x1, x2], [y1, y2], color="pink", linewidth=2.4)

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
    ax.set_aspect("equal")
    ax.axis("off")
    return width, height


if __name__ == "__main__":
    width, height = draw(plt.gca(), sys.argv[1])
    plt.gcf().set_size_inches(width, height)

    if len(sys.argv) == 2:
        plt.show()
    else:
        path = "./out"
        if not os.path.exists(path):
            os.makedirs(path)
        plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
