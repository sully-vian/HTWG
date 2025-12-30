# Practical Assignment 4: Delaunay triangulation

Implement the randomized, incremental algorithm of the lecture to compute the Delaunay triangulation of $n$ points $p_1, \dots, p_n $ in the plane.

Remark:
1. Do not use trapezoidal maps.
2. Do not use SurfaceMesh or other mesh-processing libraries.

## Commands

Run these commands, from the current directory.

To generate the `Makefile` run:

```bash
qmake6 DelaunayTriangulation.pro
```

To build the project run:

```bash
make
```

To start the project run:

```bash
./debug/DelaunayTriangulation
```

If `compile_commands.json` is needed (clangd LSP support), run:

```bash
bear -- make -j1
```

## Algorithm

```C++
// Algorithm Delaunay-Triangulation
// Input: Set P of n distinct points in the plane.
// Output: Delaunay-Triangulation D.
void delaunayTriangulation(Set<Point> P) {
    auto ch = CH(P); // convex hull
    auto rest = shuffle(P\ch); // random perm of the remaining points
    auto D = ...;  // compute init trig D by connecting rest[0] to the corners of ch
    ...; // restore the Delaunay conditions
    for (int i = 1, i < rest.size(), i++) {
        Point p = rest[i];
        ...; // find triangle (pi,pj,pk) containing p
        auto triangles = ...; // find all triangles whose Delaunay-conditions are violated, using the adjacency of triangles
        ...; // remove these triangles from D leaving a polygonal hole Delta in D
        ...; // connect all corners of Delta to p with edges to re-triangulate the hole Delta;
    }
}
```
