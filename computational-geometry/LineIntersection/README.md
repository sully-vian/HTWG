# Practical Assignment 2: Line Intersection

- [ ] Implement a Binary Search Tree which follows the requirements
- [ ] Test the Binary Search Tree (Bonus)
- [ ] Implement the sweep line algorithm

## Commands

Run these commands, from the current directory.

To generate the `Makefile` run:

```bash
qmake6 All.pro
```

To build the project run:

```bash
make
```

To start the project run:

```bash
./debug/LineIntersection
```

If `compile_commands.json` is needed (clangd LSP support), run:

```bash
bear -- make -j1
```

### Tests

The AVL tree implementation can be tested with the following commands:

```bash
make sub-AVLTreeTest-pro # build only tests

./debug/avlTreeTest # run tests

dot -Tpng tree.dot | feh - # to open the generated tree as a png
```

## The Sweep Line Algorithm

```C++
Q = sortByX(events);
L = {} // List of active segments, sorted by y-coordinate
while (!Q.empty()){
    p = Q.pop();
    if (p is left end point of horizontal segment s) {
        L.append(s);
    } else if (p is right end point of segment s) {
        L.remove(s);
    } else {
        // p.key is the x-coordinate of vertical segment s
        // with lower end point (p.key,y_l) and upper end point
        // (p.key, y_u)
        Determine all horizontal segments t in L whose
        y-coordinate t.y is in [y_l,y_u] and report the
        intersection pair (t,s);
    }
}
```

