#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "acyclic",
  "a forest",
  "tree",
  "vertices of degree one",
  "spanning tree",
  "skeleton",
  "connected graph",
  "bipartite",
  "2-partition",
  "no cycle of odd length",
  "planar",
  "without edges crossing",
  "areas",
  "regions",
  "Formula of Euler",
  $3n-6$ + " edges",
  $K_5$,
  $K_(3,3)$,
  "subgraph homeomorphic to " + $K_(3,3)$ + " or " + $K_5$,
  "adjacency matrix",
  "number of paths of lengh " + $k$ + " from " + $v_i$ + " to " + $v_j$,
  "incidence matrix",
  "adjacency list",
  "vertices neighbors",
  $O(|V|^2)$,
  $O(|V|+|E|)$,
  "breadth-first",
  "in breadth",
  "queue (FIFO)",
  "depth-first",
  "in depth",
  "stack (LIFO)",
  $O(|V|+|E|)$,
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE02: Graph teory and representations])
#set par(justify: true)

#title()

= Trees and connectivity

A graph is called #word if it does not contain a simple cycle of length $3$. Such a cycle-free graph is called #word. A #word is a connected forest. A tree contains at least two #word.

For every tree $G=(V,E)$, the fundamental relationship $|E| = |V| - 1$ applies.

A #word or #word of $G$ is a subgraph of G that is a tree and contains every node of $G$. Every #word has a spanning tree.

= Bipartite and planar graphs

A graph $G=(V,E)$ is called #word if there is a #word of $V$ into subsets $V_1$ and $V_2$ such that every edge in $G$ has one endpoint in $V_1$ and one endpoint in $V_2$. A connected graph $G$ is bipartite if and only if $G$ contains #word.

A graph is called #word if it has a diagram that can be drawn #word. Such a diagram divides the drawing plane into #word or #word.

The #word states that for every planar diagram of a connected graph with $n$ vertices, $m$ edges and $f$ faces, the following applies:

$ n-m+f=2 $.

A planar connected graph $G$ with $n gt.eq 3$ vertices has at most #word.

The smallest non-planar complete graph is #word. The complete bipartite graph #word is also non-planar.

According to Kuratowski's theorem, a graph is planar if and only if it does not contain any #word.

= Data structures and algorithms

The #word $A(G)$ of a graph with $n$ nodes is an $n times n$ matrix. The $(i,j)$-th entry of the $k$-th power of $A$ gives the #word.

The #word $B(G)$ of a graph with $n$ nodes and $m$ edges is an $n times m$ matrix.

An #word of a graph $G$ consists of lists $L_1, dots, L_N$, where the list $L_i$ contains the #word with $v_i$.

The memory requirement for an adjacency matrix is #word. The memory requirement for an adjacency list is #word.

When constructing a spanning tree:

The #word traverses nodes #word and uses a list organised as #word (‘first-in, first-out’).

The #word traverses the nodes #word and uses a list organised as #word (‘last-in, first-out’).

If the graph is represented by an adjacency list, then the runtime of both algorithms is #word.
