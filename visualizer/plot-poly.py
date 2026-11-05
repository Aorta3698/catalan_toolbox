import matplotlib.pyplot as plt
import math
import os
import sys
import time

from watchdog.observers import Observer
from watchdog.events import PatternMatchingEventHandler


class MyHandler(PatternMatchingEventHandler):
    def __init__(self):
        super().__init__(patterns=[".polylog"], ignore_directories=True)

    def on_modified(self, event):
        # https://stackoverflow.com/questions/44278369/how-to-keep-matplotlib-python-window-in-background

        with open(".polylog") as file:
            e = int(file.readline())
            u, v = file.readline().strip().split(",")
            u, v = int(u), int(v)

            ## TODO: right now, whole graph gets redrawn every flip, which is bad for performance
            idx = e + num_of_points - 1
            G[idx] = (u, v)

            draw()
            plt.gcf().canvas.draw_idle()
            plt.gcf().canvas.flush_events()


def draw():
    ax = plt.gca()
    ax.clear()
    for i, (u, v) in enumerate(G):
        (x1, y1), (x2, y2) = node_positions[u], node_positions[v]
        if i < num_of_points - 1:
            ax.plot([x1, x2], [y1, y2], color="black", alpha=0.5, linewidth=1.5)
        elif i == num_of_points - 1:
            ax.plot([x1, x2], [y1, y2], color="green", alpha=0.5, linewidth=3)
        else:
            ax.plot([x1, x2], [y1, y2], color="pink", linewidth=3)
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


if __name__ == "__main__":

    ## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
    os.environ["OPENBLAS_NUM_THREADS"] = "1"
    os.environ["XDG_SESSION_TYPE"] = "xcb"

    G = []
    node_positions = {}

    with open(sys.argv[1]) as infile:
        num_of_points = int(infile.readline())
        for i in range(num_of_points):
            line = infile.readline()
            x, y = line.strip().split(",")
            node_positions[i] = (float(x), float(y))
        for line in infile:
            u, v = line.strip().split(",")
            e = (int(u), int(v))
            G.append(e)

    width = 4.8 + 0.30 * num_of_points
    height = 3.2 + 0.30 * num_of_points
    plt.figure(figsize=(width, height))
    draw()

    handler = MyHandler()
    observer = Observer()
    observer.schedule(handler, ".", recursive=False)
    observer.start()

    if len(sys.argv) == 2:
        plt.show()
    else:
        path = "./out"
        if not os.path.exists(path):
            os.makedirs(path)
        plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
