#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "separating",
  "Menger",
  "equality",
  "edge-disjoint",
  "minimal",
  "flow network",
  "of internally vertex-disjoint",
  "vertex set",
  "matching",
  "vertex cover",
  "bipartite",
  "maximum",
  "minimum vertex",
  "Hall",
  "Marriage Theorem",
  "adjacent",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE06: Networks and graph theory])
#set par(justify: true)

#title()

= Menger and Flows

Be $D=(V,E)$ a digraph and let $q$ and $s$ be vertices in $D$. A set $E'$ of edges in $D$ is called a $q$ and $s$ #word edge set in $D$, if every directed path from $q$ to $s$ contains at least one edge from $E'$.

The theorem 22.18, known as the Theorem of #word (1927), establishes an #word : The maximum number of #word directed paths in $D$ from $q$ to $s$ is equal to the cardinality of a #word $q$ and $s$ separating edge set in $D$.

One can prove this by considering a #word on $D$, in which every edge has the capacity 1. The value of a maximum flow on this network corresponds to the maximum number of edge-disjoint paths.

For non-adjacent vertices $q$ and $s$, the vertex version of Menger's Theorem (Theorem 22.20) states that the maximum number #word directed paths in $D$ from $q$ to $s$ is equal to the cardinality of a minimal $q$ and $s$ separating #word.

= König-Egerváry and Matchings

Let $G$ be a graph. A set of edges $P$ is called a #word in $G$, if the edges in $P$ have no common endpoint.

A set $U$ of vertices in $G$ is called a #word of $G$, if for every edge $u v$ in $G$, $u in U$ or $v in U$ holds.

The theorem 22.22 (König-Egerváry, 1931) applies to #word graphs and states that the cardinality of a #word matching in $G$ is equal to the cardinality of a #word vertex cover of $G$.

= Hall and Systems of Representatives

The Theorem of #word (1935), which is also known as the #word, deals with the existence of a perfect matching in a bipartite graph.

Let $G$ be a bipartite graph with 2-partition ${V_1, V_2}$, where $|V_1| = |V_2|$. A perfect matching exists if and only if for every subset $U$ of $V_1$: $|U^+| gt.eq |U|$. In this context, $U^+$ consists of all vertices of $V_2$ that are #word with any vertex from $U$.
