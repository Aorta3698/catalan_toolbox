import matplotlib.pyplot as plt
import os
import sys
import time
from netgraph import Graph
from netgraph import InteractiveGraph

from watchdog.observers import Observer
from watchdog.events import PatternMatchingEventHandler


class MyHandler(PatternMatchingEventHandler):
    def __init__(self, plot, T):
        super().__init__(patterns=[".polylog"], ignore_directories=True)
        self.plot = plot
        self.T = T

    def on_modified(self, event):
        # https://stackoverflow.com/questions/44278369/how-to-keep-matplotlib-python-window-in-background

        with open(".polylog") as file:
            e = int(file.readline())
            u, v = file.readline().strip().split(",")
            u, v = int(u), int(v)

            ## TODO: right now, whole graph gets redrawn every flip, which is bad for performance
            ## maybe figure out this: https://github.com/paulbrodersen/netgraph/issues/20
            idx = e + num_of_points - 1
            old_e = G[idx]
            G[idx] = (u, v)
            T[idx] = (u, v)
            del edge_labels[old_e]
            edge_labels[(u, v)] = e

            self.plot.fig.clear()
            self.plot = InteractiveGraph(
                G,
                node_layout=node_positions,
                edge_labels=edge_labels,
                edge_label_fontdict=dict(size=14),
            )

            style(self.T, self.plot)
            self.plot.fig.canvas.draw_idle()
            self.plot.fig.canvas.flush_events()


def style(T, P):
    ## style root
    root = T[num_of_points - 1]
    P.edge_artists[root].set_color("green")
    P.edge_artists[root].set_alpha(0.5)
    P.edge_artists[root].update_width(0.02)

    ## style inner edges
    for e in T[num_of_points::]:
        P.edge_artists[e].set_color("pink")
        P.edge_artists[e].set_alpha(1.0)
        P.edge_artists[e].update_width(0.02)


if __name__ == "__main__":

    ## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
    os.environ["OPENBLAS_NUM_THREADS"] = "1"
    os.environ["XDG_SESSION_TYPE"] = "xcb"

    G = []
    T = []
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
            T.append(e)

    edge_labels = {}
    id = 1
    for e in T[num_of_points:]:
        edge_labels[e] = id
        id = id + 1

    width = 4.8 + 0.30 * num_of_points
    height = 3.2 + 0.30 * num_of_points
    plt.figure(figsize=(width, height))
    P = InteractiveGraph(
        G,
        node_layout=node_positions,
        edge_labels=edge_labels,
        edge_label_fontdict=dict(size=14),
    )

    style(T, P)

    handler = MyHandler(P, T)
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
