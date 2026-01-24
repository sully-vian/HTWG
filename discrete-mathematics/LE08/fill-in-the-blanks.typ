#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "does not esceed the total capacity",
  "upper bound",
  "less than or equal",
  "bounding function to truncate",
  "rational backpack",
  "listed in descending order",
  "ratio",
  "NP-hard",
  "as a simple circuit",
  "problem",
  "lower bound",
  "minimum spanning tree",
  "very large seach space",
  "quality",
  "neighborhood",
  "mapping",
  "neighboring",
  "Hamming distance",
  "Lin-2-Opt step",
  "local maximum",
  "neighborhood",
  "higher value",
  "steepest ascent if a feasible solution with maximum value",
  "no",
  "a very large number of local optima",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE08: Combinatorial optimization])
#set par(justify: true)

#title()

= Backtracking and branch-and-bound

_Rucksack−Backtrack2_, a recursive call with $x_(k+1)=1$, is only executed if the cumulative size of the already selected items plus the size of the $(k +1)$-th item #word $K_0$. A bounding function $B(x)$ is used to reduce the search space. For maximization problems, $B(x)$ must represent an #word for the maximum value $P(x)$ of all feasible solutions in the subtree with root $x$. A subtree can be pruned if the computed bounding function $B(x)$. The value of the current optimal solution , $o p t f$, is #word to the currently optimal value, since the optimal solution in this subtree cannot be improved. The main improvement of _Rucksack−Backtrack3_ over _Rucksack−Backtrack2_ is the use of a #word subtrees early. The bounding function $B(x)$ for the integer backpack problem is derived by adding the value of the partial solution $f(x)$ to the value of an optimal #word for the remaining items. To directly determine an optimal rational backpack, the items must be #word according to the #word $f_i \/ g_i$ (value per weight) should be sorted.

= Travelling Roundabout Problem (TSP)

The traveling salesman problem is computationally demanding because it is classified as #word. A traveling salesman is mathematically defined as #word in $K_n$ containing all vertices (a Hamiltonian circuit). Since the traveling salesman problem is a minimization #word, a bounding function $B(x)$ must provide a #word for the cost of all possible extensions of the partial solution $x$. The bounding function $B(x)$ for the TSP is composed of the sum of the edges already traversed and the value of the #word of the remaining subgraph.

= Heuristic algorithms

Heuristic algorithms replace backtracking when combinatorial optimization problems have a #word. The #word of an approximate solution provided by a heuristic algorithm is generally unpredictable. A #word, $N$, is defined as a #word, $N:X arrow P(X)$, that maps each solution $x in X$ to a set of #word. For the knapsack problem, two knapsacks are considered neighboring if they differ by exactly one item, a concept known as the #word ($d(x,y)=1$). In the traveling salesman problem, the neighborhood is typically defined by the #word.

A feasible solution $x^* in X$ is considered a #word if $f(x^*) gt.eq f(x)$ for all feasible $x$ in the #word $N(x^*)$ holds. In the uphill method, for a feasible solution $x$, a neighboring feasible solution $y in N(x)$ with a #word is constructed. The uphill method is called the method of #word is always chosen in each neighborhood. The generic algorithm _HeuristicSearch_ terminates prematurely if the heuristic $H_N$ #word neighboring feasible successor $y$ is found (i.e., $y = f a i l$ ). Combinatorial optimization problems typically have #word.
