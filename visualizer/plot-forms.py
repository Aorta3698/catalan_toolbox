import matplotlib.pyplot as plt
import os
import runpy
import screen
import sys
from matplotlib.widgets import Button

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

forms = [
    ("plot-tree.py", "tree"),
    ("plot-dyck-path.py", "dyck"),
    ("plot-poly.py", "triangulation"),
    ("plot-bracketing.py", "brackets"),
    ("plot-arcs.py", "arcs"),
    ("plot-chords.py", "chords"),
    ("plot-coins.py", "coins"),
]
files = sys.argv[2:]
draws = [runpy.run_path(f"./visualizer/{script}")["draw"] for script, _ in forms[: len(files)]]

fig = plt.figure()
ax = None
buttons = []


def show(i):
    global ax
    if ax:
        ax.remove()
    ax = fig.add_axes([0, 0, 1, 1])
    width, height = draws[i](ax, files[i])

    ## buttons at the bottom
    width, height = max(width, 7), max(height, 3) + 0.5
    fig.set_size_inches(width, height)
    row = 0.5 / height
    ax.set_position([0.125, row + 0.11 * (1 - row), 0.775, 0.77 * (1 - row)])
    for j, button in enumerate(buttons):
        n = len(buttons)
        button.ax.set_position([j / n + 0.005, 0.15 * row, 1 / n - 0.01, 0.7 * row])
        button.color = "lightblue" if j == i else "0.85"
        button.ax.set_facecolor(button.color)

    s = screen.fit(fig)
    for text in ax.texts:
        text.set_fontsize(text.get_fontsize() * s)
    ## reset zoom history
    if fig.canvas.toolbar:
        fig.canvas.toolbar.update()
    fig.canvas.draw_idle()


def close(event):
    for file in files:
        os.remove(file)


for i, (_, name) in enumerate(forms[: len(files)]):
    button = Button(fig.add_axes([0, 0, 0.1, 0.1]), name)
    button.on_clicked(lambda event, i=i: show(i))
    buttons.append(button)
fig.canvas.mpl_connect("close_event", close)
show(int(sys.argv[1]))
plt.show()
