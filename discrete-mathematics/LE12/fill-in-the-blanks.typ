#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "dual",
  "duality theorem",
  $b^T y^*$,
  "optimal",
  "Farkas'",
  $b^T y gt.eq 0$,
  "convex",
  "line",
  "convex hull",
  "polyhedron",
  "corners",
  "convex",
  "corners",
  "matching",
  "vertex cover",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE12: Linear optimization, duality and geometry])
#set par(justify: true)

#title()

In linear optimization, we often consider a primal problem in the form $"max" c^T x$ subject to the constraints $A x lt.eq b$ and $x gt.eq 0$. The corresponding #word problem is then: $"min" b^T y$ subject $A^T y gt.eq c$ and $y gt.eq 0$.

The fundamental #word states: If both programs have feasible solutions, then both are solvable, and for the optimal solutions $x^*$ and $y^*$ , the equality $c^T x^* =$ #word holds. If one of the programs does not have a feasible solution, then the other also does not have an #word solution.

#word Lemma (1902). For example, it states that for the set $P = { x in RR^n | A x = b, x gt.eq 0}$, it is non-empty if and only if for every $y$ such that $A^T y gt.eq 0$, then necessarily #word also holds.

Geometrically speaking, the feasible region $P$ of a linear program is a convex set. A set $A$ is called #word if, for any two points $x,y in A$, it always contains the entire #word between those points. The smallest convex set encompassing a given set $A$ is called the #word of $A$. If the generating set $A$ is finite, the result $"conv"(A)$ is called a convex #word.

The #word of the polyhedron are of particular interest for the simplex method. A point $x$ is a convex polyhedron if and only if it cannot be represented as a #word linear combination of other points from $P$; that is, it does not lie 'between' any two other points in the set. According to Theorem 24.10, a convex polyhedron is identical to the convex hull of its #word.

A concrete example of this theory is graph theory. If one seeks a maximal #word in a graph $G$, the relaxation of this integer problem leads to an LP. The dual problem is directly related to the search for a minimal #word, where each edge $e = u v$ is a constraint of the form $y_u+y_v gt.eq 1$ delivers.
