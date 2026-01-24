#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "filter",
  "matroid",
  "exchange property",
  "bases",
  "of same cardinality",
  "greedy algorithm",
  "graphic matroid",
  "forest",
  "minimum spanning tree",
  "the chromatic number",
  "2-colorable",
  "NP-hard problem",
  "order",
  "clique",
  "maximum degree",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE10: Combinatorial Optimization and graph theory])
#set par(justify: true)

#title()

A system of sets $M$ on a base set $A$ is called a #word if it is closed with respects to inclusion. Such a system is called a #word if it also satisfies the so-called #word. The independant sets with the maximum number of elements within a matroid are called #word. An important property of finite matroids is that all these sets are always #word.

A significant result of optimization states that a #word for a filter $M$ finds an optimal solution for any cost function if and only if $M$ is a matroid. A classic example of this is the #word matroid of a graph, where the independant sets are the edge sets that spann a #word. Applying the greedy principle to this always yields a #word.

In graph theory, the topic of vertex coloring is studied. The minimum number of colors required to color the vertices of a graph such that adjacent vertices have different colors is called #word. A graph is #word if it is bipartite. While determining this number exactly is an #word greedy method is often used. However, the effectiveness of this method depends strongly on the chosen #word of the vertices.

Mathematically, the number of colors can be limited by other graph parameters. It is at least as large as the number of nodes in a maximal #word. It is limited from above by the #word. $Delta(G)$ plus one is bounded. For connected, non-regular graphs, this bound can even be improved to $chi(G) lt.eq Delta(G)$.
