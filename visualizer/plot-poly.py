import matplotlib.pyplot as plt
import math
import os
import sys
import time


def load():
    global num_of_points
    G.clear()
    node_positions.clear()
    tree_positions.clear()
    tree_edges.clear()

    with open(sys.argv[1]) as infile:
        num_of_points = int(infile.readline())
        for i in range(num_of_points):
            line = infile.readline()
            x, y = line.strip().split(",")
            node_positions[i] = (float(x), float(y))
        for line in infile:
            if line.strip() == "tree":
                break
            u, v = line.strip().split(",")
            e = (int(u), int(v))
            G.append(e)
        if infile.readline():
            for _ in range(int(infile.readline())):
                v, x, y = infile.readline().strip().split(",")
                tree_positions[int(v)] = (float(x), float(y))
            for line in infile:
                u, v = line.strip().split(",")
                tree_edges.append((int(u), int(v)))


def check():
    global stamp
    if not os.path.exists(".polylog") or os.stat(".polylog").st_mtime_ns == stamp:
        return
    try:
        load()
    except ValueError:
        return
    stamp = os.stat(".polylog").st_mtime_ns

    ## TODO: right now, whole graph gets redrawn every flip, which is bad for performance
    draw()
    plt.gcf().canvas.draw_idle()


def on_click(event):
    for line, k in diagonals.items():
        if line.contains(event)[0]:
            print(k, flush=True)
            return


def draw():
    ax = axs[0]
    ax.clear()
    diagonals.clear()
    for i, (u, v) in enumerate(G):
        (x1, y1), (x2, y2) = node_positions[u], node_positions[v]
        if i < num_of_points:
            ax.plot([x1, x2], [y1, y2], color="black", alpha=0.5, linewidth=1.5)
        else:
            line = ax.plot([x1, x2], [y1, y2], color="pink", linewidth=3, pickradius=7)[0]
            diagonals[line] = i - num_of_points + 1
            if not tree_edges:
                continue
            angle = math.degrees(math.atan2(y2 - y1, x2 - x1))
            if angle > 90:
                angle -= 180
            elif angle < -90:
                angle += 180
            ax.text(
                (x1 + x2) / 2,
                (y1 + y2) / 2,
                i - num_of_points + 1,
                fontsize=14,
                rotation=angle,
                rotation_mode="anchor",
                ha="center",
                va="center",
                bbox=dict(boxstyle="round", ec="white", fc="white"),
            )

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

    if tree_edges:
        draw_tree(axs[1])


def draw_tree(ax):
    ax.clear()
    parents = {u for u, _ in tree_edges}
    for u, v in tree_edges:
        (x1, y1), (x2, y2) = tree_positions[u], tree_positions[v]
        ax.plot([x1, x2], [y1, y2], color="black", alpha=0.5, linewidth=1.5)

    for v, (x, y) in tree_positions.items():
        ax.plot(
            x,
            y,
            "o",
            markersize=22 if v in parents else 8,
            markerfacecolor="white",
            markeredgecolor="black",
            markeredgewidth=0.75,
        )
        if v in parents:
            ax.text(x, y, v, fontsize=12, ha="center", va="center_baseline")
    ax.axis("off")


if __name__ == "__main__":

    ## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
    os.environ["OPENBLAS_NUM_THREADS"] = "1"
    os.environ["XDG_SESSION_TYPE"] = "xcb"

    G = []
    node_positions = {}
    tree_positions = {}
    tree_edges = []
    diagonals = {}
    load()

    width = 4.8 + 0.30 * num_of_points
    height = 3.2 + 0.30 * num_of_points
    if tree_edges:
        fig, axs = plt.subplots(1, 2, figsize=(2 * width, height))
    else:
        fig, ax = plt.subplots(figsize=(width, height))
        axs = [ax]
    draw()

    if tree_edges:
        stamp = None
        timer = fig.canvas.new_timer(interval=100)
        timer.add_callback(check)
        timer.start()
        fig.canvas.mpl_connect("button_press_event", on_click)

    if len(sys.argv) == 2:
        plt.show()
    else:
        path = "./out"
        if not os.path.exists(path):
            os.makedirs(path)
        plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
