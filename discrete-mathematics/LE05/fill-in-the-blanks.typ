#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "flow value",
  "source",
  "sink",
  "the complement of " + $S$,
  "smaller than or equal to",
  "Ford-Fulkeson",
  "minimal",
  "iterative",
  "forward",
  "backward",
  "positive",
  "minimum",
  "Flow Mark",
  "Flow Increase",
  "minimal cut",
  "integer",
  "irrational",
  "Edmonds & Karp",
  "breadth-first search",
  "integer",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE05: Maximum flows])

#title()

Fill-in-the-blank text: Maximum flows

= Definitions and fundamental theorem

A flow neto $N=(D,kappa,q,s)$ consists of a directed graph $D$. a capacity function $kappa$, the source q$$ and the sink $s$. The number $omega(f)$ is called the #word and characterizes the total flow through the network. A flow fis called maximal on $N$ if for every other flow $f'$, the value of $f'$ is less than or equal to the value of $f$ ($omega(T)lt.eq omega(U)$).

A subset $S$ of the node set $V$ is called a cut in $N$ if $S$ contains #word $q$ but not the #word $s$. The capacity of a cut $S$, denoted by $kappa(S)$ is the sum of the capacities of the edges running from nodes in $S$ to nodes in #word ($V backslash S$).

The central theoretical result is stated in Lemma 22.13, which says that for every flow $f$ and every cut $S$, the flow value $omega(f)$ is always #word the cut capacity $kappa(S)$ ($omega(f) lt.eq kappa(S)$).

Theorem 22.15 of #word (Max-Flow Min-Cut Theorem) states that the value of a maximum flow is equal to the capacity of a #word cut.

= The iterative algorithm

The proof of the max-flow min-cut theorem provides an algorithm for calculating the maximum flow.

In each step a #word path $X$ from $q$ to $s$ is constructed that allows the current flow $f$ to be increased. This path can contain two types of edges:
1. #word where the capacity $kappa$ is not yet fully utilized ($kappa(e)>f(e)$).
2. #word word where the current flow $f(e)$ is #word ($f(e)>0$).

The flow in provement $epsilon$ is defined as the #word of all available capacities or flows along this path.

The algorithm is implemented by the two routines #word (Algorithm 22.4) and #word (Algorithm 22.5)

If the routine Flow Mark no longer reaches the sink $s$, the maximum flow has been reached, and the set $S$ of marked nodes then forms the #word.

= Termination

For flow networks whose capacity values are #word or rational, the algorithm terminates reliably since the flow is increased by a positive integer at each step.

With #word capacity values, termination may fail. To remedy this and guarantee finite time requirements, the algorithm was modified by #word so that the flow-increasing path is found via #word. The Theorem 22.17 on integer values states that with integer capacities an #word maximum flow also exists.
