import matplotlib.pyplot as plt
import os
import runpy
import sys

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

## the seven forms, in Figure 1.1's order
forms = [
    ("plot-tree.py", "(a) full binary tree"),
    ("plot-dyck-path.py", "(b) Dyck path"),
    ("plot-poly.py", "(c) triangulation"),
    ("plot-bracketing.py", "(d) bracketing"),
    ("plot-arcs.py", "(e) arc diagram"),
    ("plot-chords.py", "(f) chord diagram"),
    ("plot-coins.py", "(g) coin stack"),
]

## four on top, three below
fig = plt.figure(figsize=(16, 7))
grid = fig.add_gridspec(2, 8)
panels = [fig.add_subplot(grid[0, 2 * i : 2 * i + 2]) for i in range(4)]
panels += [fig.add_subplot(grid[1, 2 * i + 1 : 2 * i + 3]) for i in range(3)]

for ax, (script, caption), data in zip(panels, forms, sys.argv[1:]):
    runpy.run_path(f"./visualizer/{script}")["draw"](ax, data)
    cell = ax.get_subplotspec().get_position(fig)
    fig.text(cell.x0 + cell.width / 2, cell.y0 - 0.01, caption, ha="center", va="top")

## fit text in its panel
renderer = fig.canvas.get_renderer()
for ax in panels:
    for text in ax.texts:
        width = text.get_window_extent(renderer).width
        if width > ax.bbox.width:
            text.set_fontsize(text.get_fontsize() * ax.bbox.width / width)

plt.show()
