# Practical Assignment 2: Line Intersection

- [ ] Implement a Binary Search Tree which follows the requirements
- [ ] Test the Binary Search Tree (Bonus)
- [ ] Implement the sweep line algorithm

## Commands

Run these commands, from the current directory.

To generate the `Makefile` files run:

```bash
qmake6 LineIntersection.pro
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

## The Sweep Line Algorithm

### Algorithm 1: Intersect iso-oriented line segments

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

### Algorithm 2: Find knot p with smallest key >= y

```C++
p = root;
q = NULL;
while ((p != NULL) && (p.key != y)) {
    q = p;
    if (y < p.key) {
        p = p.leftChild;
    } else {
        p = p.rightChild;
    }
    if (p != NULL) { // key==y found
        return p; 
    } else if (root == NULL) { // empty tree
        return NULL;
    } else if (q == root) { // tree has only one knot
        return NULL or root depending on root.key;
    } else if (y < q.key) {
        return q;
    } else {
        return successor(q);
    }
}
```
