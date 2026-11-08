import matplotlib.pyplot as plt
import os
import sys

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"

with open(sys.argv[1]) as infile:
    bracketing = infile.readline().strip()

## one line per 80 characters
lines = [bracketing[i : i + 80] for i in range(0, len(bracketing), 80)]

## set image dimension
width = 1.6 + 0.15 * len(lines[0])
height = 0.6 + 0.5 * len(lines)
plt.figure(figsize=(width, height))

## draw it as math, like the thesis
plt.rcParams["mathtext.fontset"] = "cm"
ax = plt.gca()
text = "\n".join(f"${line}$" for line in lines)
ax.text(0.5, 0.5, text, fontsize=20, ha="center", va="center", multialignment="left")
ax.axis("off")

if len(sys.argv) == 2:
    plt.show()
else:
    path = "./out"
    if not os.path.exists(path):
        os.makedirs(path)
    plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
