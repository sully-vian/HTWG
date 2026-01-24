#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "NP-hard problems",
  "exponential",
  "polynomial",
  "infeasible",
  "decision problem",
  $P$,
  "verifiable in polynomial time",
  $N P$,
  "NP-complete",
  $P = N P$,
  "answers",
  "polynomial",
  "satisfiability problem",
  "clique",
  "NP-complete",
  "feasible solutions",
  "the search-space",
  "NP-hard problem",
  "Maximization",
  "minimizing",
  "backtracking algorithm",
  "NP-hard",
  "branch-and-bound",
  "feasible",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE07: Combinatorial optimization])
#set par(justify: true)

#title()

Many optimization problems of great economic importance are so complex that an exact solution is often impossible. These problems typically belong to the class of #word and are characterized by the fact that their solution requires #word effort.

To solve practical problems, algorithms with #word runtime ($O(n^d)$) are needed, since algorithms with exponential runtime ($O(c^n)$) are mostly #word.

= Complexity classes

A #word requires a "yes" or "no" answer. The class #word consists of all decision problems that are solvable in polynomial time. The class $N P$ encompasses decision problems for which a proof of the affirmative answer is #word. Obviously, $P subset.eq$ #word.

A decision problem $D$ is called #word if $D in N P$ and for every problem $D in N P, D' prop_T D$. If such a problem $D$ were in the class $P$, then this would imply: #word.

The transformation from $D$ to $D'$, in which a polynomial algorithm transforms each instance $I$ of $D$ into an instance $I'$ of $D'$ such that the #word match, is called a #word transformation.

An example of an $N P$-complete problem is the #word, which was first proven by Stephen A. Cook.

= Graph problems and optimization

A subset $U subset.eq V$ in a graph $G=(V,E)$ is called a #word if $u v in E$ for all $u, v in U$. The problem of finding a clique with maximum cardinality is an optimization problem. #word decision problems are the analogue of $N P$-hard optimization problems.

In combinatorial optimization, the finite set whose elements are called #word in practitioner jargon as #word. The optimization variant of finding a maximal clique in a graph is an #word. #word problems are equivalent, since maximizing an objective function $f$ is equivalent to #word f.

= Solution methods

A #word is a recursive procedure that generates all solutions to a combinatorial optimization problem step by step. An example is the _knapsack-backtrack1_ algorithm for the knapsack problem. The knapsack problem is #word and asks for a knapsack with maximum value whose size does not exceed the capacity $K_0$.

To improve backtracking algorithms, the technique of #word used. This is possible if the root of the subtree (the partial solution) is not #word, since then all partial solutions in that subtree are also not feasible.
