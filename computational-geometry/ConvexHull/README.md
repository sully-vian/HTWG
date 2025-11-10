# Practical Assignment 1: Convex hull computation

- [X] Graham's Scan
- [X] Jarvis' March
- [ ] Chan's Algorithm (Bonus)

## Commands

Run these commands, from the current directory.

To generate the `Makefile` files run:

```bash
qmake6 ConvexHull.pro
```

To build the project run:

```bash
make
```

To start the project run:

```bash
./debug/ConvexHull
```

If `compile_commands.json` is needed (clangd LSP support), run:

```bash
bear -- make -j1
```

## Graham's Scan


Algorithm in pseudocode:

```C++
Polygon GrahamScan(Points P) {
    (p1, ..., pn) = sortByX(P);
    L_upper.append(p1);
    L_upper.append(p2);
    for (i=3; i<=n; i++) {
        L_upper.append(pi);
        while (L_upper.size() > 2 && the last three points a,b,c make a left turn) {
            L_upper.delete(b); // middle one
        }
    }
    L_lower.append(pn);
    L_lower.append(pn-1);
    for (i=n-2, i>=1; i--) {
        L_lower.append(pi);
        while (L_lower.size() > 2 && the last three points a,b,c make a left turn) {
            L_lower.delete(b); // middle one
        }
    }
    L_lower.delete(first point);
    L_lower.delete(last point);
    return L_upper.append(L_lower);
}
```

## Jarvis' March

Algorithm in pseudocode:


```C++
Polygon JarvisMarch(Points P) {
    p1 = lexicographicMin(P);
    q1 = p1;
    i = 2
        qi = end point of line (p1,q1) with smallest angle to horizontalline through p1;
    repeat {
        i++;
        qi = end point of line (qi-1,qi) with smallest angle to line (qi-2,qi-1) et qi-1;
    } until (qi == pi);
    L = (q1, ..., qk);
    return L;
}
```

## Chan's Algotihm

Algorithm in pseudocode:

```C++
Polygon ChansAlgorithm(Point P) {
    for (t=1,2,... until hull is found) {
        H = min(2^2^t, n) // estimate for number corners h
        Partition P into k=n/m subsets P1, ..., Pk of sizes <= m; // Pre-processing
        Compute CH(P1), ..., CH(Pk); // using Grahan's scan
        p1 = lexicographicMin(P);
        p0 = (-∞, 0);
        for (j=1, ..., H) { // try to find H-corner-hull
            for (i=1, ..., k) { // wrapping of CH(P1), ..., CH(Pk)
                Compute qi∈CH(Pi)\{Pj} that maximizes ∢(pj-1,pj,pi) using binary search on CH(Pi); // CH(Pi) already "sorted"
            }
            pj+1 = argmax(q∈{q1, ..., qk} that maximizes ∢(pj-1,pj,q));
            if (pj+1 = p1) {
                return hull is found;
            }
        } // H too small, could not find H-corner-hull
    }     // improve estimate for h
}
```
