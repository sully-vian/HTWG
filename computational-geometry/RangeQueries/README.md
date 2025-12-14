# Practical Assignment 3: Range Queries

- [x] Implement the 2D-Tree data structure
- [x] Test the 2D-Tree data structure
- [x] Implement the interactive construction of the 2D-Tree
- [x] Implement interactive range search for rectangular query

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
./debug/RangeQueries
```

If `compile_commands.json` is needed (clangd LSP support), run:

```bash
bear -- make -j1
```

## Tests

```bash
make sub-KDTreeTest-pro # build only tests

./debug/kdTreeTest # run tests

dot -Tpng tree.dot | feh - # to open the generated tree as a png
```

## Algorithms:

```C++
rangeSearch(knot k, direction d, queryRange D) {
    if (k ≠ NULL) {
        if (d == vertical) {
            (l, r) = (D.y1, D.y2);
            coord = k.y;
            dNew = horizontal;
        } else {
            (l, r) = (D.x1, D.x2);
            coord = k.x;
            dNew = vertical;
        }
        if (k ∈ D) {
            Add k to the output;
        }
        if (l < coord) {
            rangeSearch(k.left, dNew, D);
        }
        if (r > coord) {
            rangeSearch(k.right, dNew, D);
        }
    }
}
```

```C++
constructBalanced2DTree(leftIndex l, rightIndex r, knot k, direction d) {
    if (l <= r) {
        m = abs(l+r)/ 2;
        if (d == vertical) {
            k.value = Y[m];
            partitionField(X, l, r, m);
        } else {
            k.value = X[m];
            partitionField(Y, l, r, m);
        }
        constructBalanced2dTree(l, m-1, k.left, !d);
        constructBalanced2dTree(m+1, r, k.right, !d);
    }
}
```

```C++
findSplitNode(tree T, left bound xl, right bound xr) {
    // Output:
    // Node where paths to xl and xr split, or the leaft where both paths end
    v = root(T);
    while ((v is not a leaf) && (xl > xv or xr <= xv)) {
        if (xr <= xv) {
            v = leftChild (v);
        } else {
           v = rightChild(v);
        }
    }
    return v;
}
```

```C++
rangeSearch1D(tree T, leftBound xl, rightBound xr) {
    // Output:
    // All points in T that lie in the interval [xl,xr]
    vsplit = findSplitNode(T, xl, xr);
    if (vsplit is a leaf) {
        return the point in vsplit, if necessary;
    } else {
        v = leftChild(vsplit);
        while (v is not a leaf) {
            if (xl <= xv) {
                reportSubtree(rightChild(v));
                v = leftChild (v);
            } else {
                v = rightChild(v);
            }
        }
        return the point in v, if necessary;
        // Analogous for rightChild(vsplit) and xr;
        v = rightChild(vsplit);
    }
}
```

```C++
rangeSearch2D(tree T, queryRange (xl, xr, yl, yr)) {
    // Output:
    // All point in T that lie in the 2D query range [xl,xr]x[yl,yr]
    vsplit = findSplitNode(T, xl, xr);
    if (vsplit is a leaf) {
        return the point in vsplit, if necessary;
    } else {
        v = reftChild(vsplit);
        return the point in v, if necessary;
        while (v is not a leaf) {
            if  (xl <= xv) {
                rangeSearch1D(tassoc(rightChild(v)), yl, yr);
                v = leftChild (v);
            } else {
                v = lightChild(v);
            }
        }
        // Analogous for RightChild(vsplit) and xr;
    }
}
```
