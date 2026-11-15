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
    ("plot-tree.py", "tree", "(a) full binary tree"),
    ("plot-dyck-path.py", "dyck", "(b) Dyck path"),
    ("plot-poly.py", "triangulation", "(c) triangulation"),
    ("plot-bracketing.py", "brackets", "(d) bracketing"),
    ("plot-arcs.py", "arcs", "(e) arc diagram"),
    ("plot-chords.py", "chords", "(f) chord diagram"),
    ("plot-coins.py", "coins", "(g) coin stack"),
]
files = sys.argv[2:]
draws = [runpy.run_path(f"./visualizer/{script}")["draw"] for script, _, _ in forms[: len(files)]]

names = [name for _, name, _ in forms[: len(files)]]

fig = plt.figure()
drawn = []
buttons = []


def show(i):
    for artist in drawn:
        artist.remove()
    drawn.clear()

    if i < len(files):
        ax = fig.add_axes([0, 0, 1, 1])
        width, height = draws[i](ax, files[i])
        width, height = max(width, 7), max(height, 3) + 0.5
        fig.set_size_inches(width, height)
        row = 0.5 / height
        ax.set_position([0.125, row + 0.11 * (1 - row), 0.775, 0.77 * (1 - row)])
        panels = [ax]
    else:
        ## every form at once
        width, height = (16, 7.5) if len(files) == 7 else (12, 6.5)
        fig.set_size_inches(width, height)
        row = 0.5 / height
        if len(files) == 7:
            grid = fig.add_gridspec(2, 8, bottom=row + 0.06, top=0.98)
            panels = [fig.add_subplot(grid[0, 2 * j : 2 * j + 2]) for j in range(4)]
            panels += [fig.add_subplot(grid[1, 2 * j + 1 : 2 * j + 3]) for j in range(3)]
        else:
            grid = fig.add_gridspec(1, 2, bottom=row + 0.06, top=0.98)
            panels = [fig.add_subplot(grid[0, j]) for j in range(2)]
        for j, panel in enumerate(panels):
            draws[j](panel, files[j])
            cell = panel.get_subplotspec().get_position(fig)
            x, y = cell.x0 + cell.width / 2, cell.y0 - 0.01
            drawn.append(fig.text(x, y, forms[j][2], ha="center", va="top"))
    drawn.extend(panels)

    ## buttons at the bottom
    for j, button in enumerate(buttons):
        n = len(buttons)
        button.ax.set_position([j / n + 0.005, 0.15 * row, 1 / n - 0.01, 0.7 * row])
        button.color = "lightblue" if j == i else "0.85"
        button.ax.set_facecolor(button.color)

    s = screen.fit(fig)
    ## fit text in its panel
    renderer = fig.canvas.get_renderer()
    for panel in panels:
        for text in panel.texts:
            text.set_fontsize(text.get_fontsize() * s)
            w = text.get_window_extent(renderer).width
            if w > panel.bbox.width:
                text.set_fontsize(text.get_fontsize() * panel.bbox.width / w)
    ## reset zoom history
    if fig.canvas.toolbar:
        fig.canvas.toolbar.update()
    fig.canvas.draw_idle()


def close(event):
    for file in files:
        os.remove(file)


for i, name in enumerate(names + ["all"]):
    button = Button(fig.add_axes([0, 0, 0.1, 0.1]), name)
    button.on_clicked(lambda event, i=i: show(i))
    buttons.append(button)
fig.canvas.mpl_connect("close_event", close)
show(int(sys.argv[1]))
plt.show()
