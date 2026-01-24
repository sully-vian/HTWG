#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "chaotic polynomial",
  "integer coefficients",
  "cliques/complete graphs",
  "backtracking",
  "climb",
  blank, // TODO
  "matroid",
  "rank",
  "monotonicity",
  "Programming",
  "feasible solutions",
  "dual LP",
  "optimal",
  blank, // TODO
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE11: Fundamental optimization])
#set par(justify: true)

#title()

In graph theory, the #word $p G(k)$ is the number of ways to color a graph with $k$ valid colors. Such a polynomial always has #word. For practical calculation, an iterative procedure can be used in which the graph is successively reduced to #word ($K_n$).

For complex optimization problems, such as the set coverage problem or the knapsack problem, various algorithms are often used. While #word seeks an exact solution, heuristic methods such as hill #word or #word use approximate approaches.

A central concept in combinatorial optimization is the #word. For a subset $T$ of a matroid, the #word $"rg"(T)$ is defined, which, among other things, possesses the property of #word. (from $T subset.eq S$ follows that $"rg"(T) lt.eq "rg"(S)$).

Linear #word aims to maximize or minimize an objective function while adhering to constraints. The set of all points that satisfy the constraints $A x lt.eq b$ and $x gt.eq 0$ is called the #word. For every primal linear program (LP), there exists a corresponding #word.

An important result here is Lemma 24.1: For a feasible solution $x$ of the primal LP and a feasible solution $y$ of the dual LP, the relation $c^T x lt.eq b^T y$ always holds. If, for two feasible solutions, $c^T x = b^T y$, then both solutions are #word for their respective problems. Many well-known theorems of graph theory, such as the #word or König-Egerváry theorems, can be interpreted as special cases of this duality theorem.
