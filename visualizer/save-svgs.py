import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import os
import runpy
import sys

script = sys.argv[1]
with open(sys.argv[2]) as infile:
    lines = infile.read().splitlines()

for count, line in enumerate(lines, 1):
    data, name = line.split(" ", 1)
    sys.argv = [script, data, name]
    runpy.run_path(script, run_name="__main__")
    plt.close("all")
    os.remove(data)
    print(f"\r{count} svg done!", end="", flush=True)
