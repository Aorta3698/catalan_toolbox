# Compile Instruction
## Python Packages To Install
- `Networkx`
- `Matplotlib`
- `Pydot`

## Install graphviz
### MacOS
```
brew install graphviz
```

## C++23
The `Makefile` uses `-std=c++23`, so make sure your `g++` compiler version supports it.

## Steps To Follow
- First create a Python venv named `visualizer` 
- Install all needed dependenceis listed way above
- Enter `make` into the terminal

# Usage
- Convert between dyck path bit string and tree in no worse than `O(nlog(n/k))`.
- Output full `k`-ary tree as a file and read from it to recreate it.
- Plot the `k`-ary tree.

----
### Possible Related Works (for my own ref)
#### Height of Tree
- https://algo.inria.fr/flajolet/Publications/FlOd82height.pdf
- https://www.cambridge.org/core/services/aop-cambridge-core/content/view/C30DF2A9C26541526D847E0A0D46F2A8/S1446788700004432a.pdf/on-the-height-of-trees.pdf
- https://math.stackexchange.com/questions/660698/variance-of-height-of-tree
- https://codeforces.com/blog/entry/95463
#### Tree Generation
- https://www.scirp.org/pdf/am_2021122213413624.pdf
- https://math.stackexchange.com/questions/145515/counting-k-ary-labelled-trees (it makes sense that it matches mine!)
- https://demonstrations.wolfram.com/PruferCodesOfLabeledTrees/ (just for fun)
- https://mathworld.wolfram.com/PrueferCode.html
- https://cp-algorithms.com/graph/pruefer_code.html
#### Queries
- https://cp-algorithms.com/data_structures/sqrt_decomposition.html#mos-algorithm
- Segment Tree, duh!
