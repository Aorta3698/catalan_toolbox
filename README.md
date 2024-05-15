# Current Direction
Implement a tool to convert between tree to other well-known (generalized) Catalan structures in ideally linear time. Common operations such as triangulation flip are to be supported for each type of structures.

---
# Galleries
Here shows the visualization for `catalan(4)` in each Catalan structure. There are 14 of them for each. 

For `catalan(5)`, please click [here](galleries/c5/README.md) (42 of them for each Catalan structure).

![](galleries/c4/tree.svg)
![](galleries/c4/poly.svg)
![](galleries/c4/chords.svg)
![](galleries/c4/arcs.svg)
![](galleries/c4/dyck_path.svg)
![](galleries/c4/coins.svg)

# For MacOS users
You will likely encounter this error below:
```
nextEventMatchingMask should only be called from the Main Thread!
```
I don't have much time to look into this now, but I believe it is because that my visualizer is not started on the main thread and apparently Apple does not allow that - read more about it [here](https://forums.developer.apple.com/forums/thread/659010).

---
# I want to know
For any Catalan structures, how do we perform a "walk" (from one state to the next) deterministically in `O(1)` while guarantees that no state gets walked on again?

Obviously, dynamic programming is a way to generate all, say, binary trees, then we can convert to whatever Catalan structure we want, but there is no walk involved (it does not go from one state to the next).

I guess, in the case of triangulation, does there always exist a `Hamiltonian Path` on the graph formed by triangulation flips? I assume the answer is yes, given that it is a graph where each vertex has `n-3` edges, but I have no concrete proof now nor know if there is a special case for such graph that a Hamiltonian Path can be found quickly.

Note: upon some googling - it looks like enumeration of Catalan structures have been studied.

---
# Milestones
### Generalized Catalan Tree generator
- [x] A verifiably random generator for full `k`-ary tree

### Tree
- [x] Visualizer for full `k`-ary trees
- [x] Gallery for $C_4$ and $C_5$ (for now)

### Dyck Path
- [x] Convert between `r`-dyck path *post-order* bit string and tree in `O(n)`
- [x] Convert between `r`-dyck path *pre-order* bit string and tree in `O(nlog(n/r))`
- [x] Visualizer for `r`-dyck path
- [x] Verify a given path is a valid `r`-dyck path or not
- [ ] Flip mountain
- [x] Gallery for $C_4$ and $C_5$ (for now)

### Triangulation
- [x] Convert between polygon triangulation and binary tree in `O(n)`
- [x] Visualizer for polygon triangulation
- [x] Triangulation flip in `O(1)` with `O(n)` pre-processing and visualize the result
- [x] Verify a given polygon triangulation is a valid polygon triangulation or not
- [x] Gallery for $C_4$ and $C_5$ (for now)

### Non-intersecting Chord Graph
- [x] Convert between non-intersecting chord graph and binary tree in `O(n)`
- [x] Visualizer for non-intersecting chord graph
- [ ] Exchange a chord and visualize the result
- [ ] Verify a given chord graph is a valid non-intersecting chord graph or not
- [x] Gallery for $C_4$ and $C_5$ (for now)

### Non-crossing Arcs
- [x] Convert between non-crossing arcs and binary tree in `O(n)`
- [x] Visualizer for non-crossing arcs 
- [ ] Exchange an arc and visualize the result
- [ ] Verify a given arcs is a valid non-crossing arcs or not
- [x] Gallery for $C_4$ and $C_5$ (for now)

### Coin Stack
- [x] Visualizer for coin stack
- [x] Gallery for $C_4$ and $C_5$ (for now)

### Other
- [x] Output full `k`-ary tree as a file and read from it to recreate it
- [ ] A website that hosts all visualization for well-known Catalan structures for `n >= 3 to n < ?`.

### Organization
- [ ] Code base rewrite to follow OOP
- [ ] Documentation

---
# Compile Instruction
## Python Packages To Install
- `Networkx`
- `Matplotlib`
- `Pydot`
- `PyQt6`
- `Netgraph`
- `Watchdog`

## C++23
The `Makefile` uses `-std=c++23`, so make sure your `g++` compiler version supports it.

## Steps To Follow
- First create a Python venv named `visualizer` 
- Install all needed dependenceis listed way above
- Enter `make` into the terminal

## How to Update Python venv
```
python -m venv /path/to/new/virtual/environment
```

## RuntimeWarning: invalid value encountered in divide
Netgraph lib currently has this warning as discussed by the library creator [here](https://github.com/paulbrodersen/netgraph/issues/81); to suppress this harming warning, put
```
np.seterr(divide='ignore', invalid='ignore')
```
where the warning occurrs.

credit to this [post](https://stackoverflow.com/questions/14861891/runtimewarning-invalid-value-encountered-in-divide).

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
