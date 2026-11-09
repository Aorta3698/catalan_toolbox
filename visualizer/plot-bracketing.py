import matplotlib.pyplot as plt
import os
import screen
import sys

## https://stackoverflow.com/questions/52026652/openblas-blas-thread-init-pthread-create-resource-temporarily-unavailable
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["XDG_SESSION_TYPE"] = "xcb"


def draw(ax, file):
    with open(file) as infile:
        bracketing = infile.readline().strip()

    ## one line per 80 characters
    lines = [bracketing[i : i + 80] for i in range(0, len(bracketing), 80)]

    ## set image dimension
    width = 1.6 + 0.15 * len(lines[0])
    height = 0.6 + 0.5 * len(lines)

    ## draw it as math, like the thesis
    plt.rcParams["mathtext.fontset"] = "cm"
    text = "\n".join(f"${line}$" for line in lines)
    ax.text(0.5, 0.5, text, fontsize=20, ha="center", va="center", multialignment="left")
    ax.axis("off")
    return width, height


if __name__ == "__main__":
    width, height = draw(plt.gca(), sys.argv[1])
    plt.gcf().set_size_inches(width, height)

    if len(sys.argv) == 2:
        s = screen.fit(plt.gcf())
        ## shrink the text with the window
        for text in plt.gca().texts:
            text.set_fontsize(text.get_fontsize() * s)
        plt.show()
    else:
        path = "./out"
        if not os.path.exists(path):
            os.makedirs(path)
        plt.savefig(f"{path}/{sys.argv[2]}.svg", bbox_inches="tight")
