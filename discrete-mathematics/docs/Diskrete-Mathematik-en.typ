#import "@preview/algorithmic:1.0.7"
#import algorithmic: algorithm-figure, style-algorithm
#show: style-algorithm

// LaTeX look
#set page(
  margin: 1.75in,
  header: context {
    let page-counter = counter(page).display((num, ..) => numbering("1", num + 270))
    let headings = query(selector(heading).before(here()))

    if headings.len() > 0 {
      let current = headings.last()
      [
        #page-counter #h(1em) #counter(heading).display(current.numbering) #current.body
      ]
    }
  },
)
#set par(leading: 0.55em, spacing: 0.55em, first-line-indent: 1.8em, justify: true)
#set text(font: "New Computer Modern", size: 10pt)
#show raw: set text(font: "New Computer Modern Mono")
#show heading: set block(above: 1.4em, below: 1em)

#show heading.where(level: 2): set block(above: 2em, below: 1em)
#show heading.where(level: 3): set block(above: 2em, below: 1em)
#show heading.where(level: 1): it => [#pagebreak()
  #block(below: 10em)[
    #counter(heading).display(it.numbering)
    #v(-1em)
    #line(length: 100%)
    #it.body
  ]
]
#show heading.where(level: 3): it => block(above: 2em, below: 2em)[#it.body] // skip level 3 numbering
#show figure.caption: it => [Fig. #it.body]

#set document(author: "Vianney Hervy", title: [Graphs and Optimization])
#set heading(numbering: (..nums) => {
  let values = nums.pos()
  if values.len() == 1 {
    numbering("1", values.at(0) + 20)
  } else {
    numbering("1.1", values.at(0) + 20, ..values.slice(1))
  }
})

#let qed = $square.stroked.small$
#let pf(body) = [*Proof.* #body #qed]
#let th(num, body) = [*Theorem #num.* #body]
#let ex(num, body) = [*Example #num.* #body]
#let cl(num, body) = [*Corollary #num.* #body]
#let lm(num, body) = [*Lemma #num.* #body]

#title()

#pagebreak()

= Graphs

Graph theory is an important tool for tackling complex problems in various scientific fields. The interdisciplinary nature of graph theory stems from the structure of graphs. Graph theory is applicable whenever a problem involves pairs of objects that are in relation to each other. Examples include road networks, electrical networks, and flow diagrams. In this chapter, fundamental concepts of graph theory are discussed.

== Basic Concepts

A graph is a pair $G = (V, E)$ consisting of a non-empty set $V$ and a set $E$ of 2-element subsets of $V$ . The elements of $V$ are called vertices and the elements of $E$ are called edges. An edge $e = {u, v}$ is also written as a word $e = u v$ (or $e = v u$). If an edge $e = u v$ exists, then $u$ and $v$ are incident with $e$, $u$ and $v$ are adjacent (or directly connected), and $u$ and $v$ are the endpoints of $e$.

In the following, only finite graphs are considered, i.e., graphs with a finite vertex set. The number of vertices of a graph $G$ is called the order of $G$ and the number of edges of $G$ is called the size of $G$.

A graph can be represented by a diagram in which vertices are represented by points in the plane and edges by continuous line segments.

#ex 21.1. The graph G with vertex set $V = {v_1, dots, v_4}$ and edge set $E = {v_1v_3, v_2v_3, v_2v_4, v_3v_4}$ is represented by the diagram in Fig. 21.1.

A graph $G = (V, E)$ has neither loops nor multiple edges. Loops are 1-element subsets of $V$, i.e., edges that are incident with only one vertex. Multiple edges are multisets of 2-element subsets of $V$, i.e., multiple connections between two vertices.

#figure("TODO", caption: [Fig. 21.1. A diagram of the graph in 21.1.])

Let $G = (V, E)$ be a graph. The number of edges incident with a vertex $v in V$ is called the degree of $v$ and denoted by $d(v)$. If $d(v) = 0$, then $v$ is isolated. If all vertices in $G$ have the same degree $k$, then $G$ is called $k$-regular. \
#th(21.2)[(Handshaking Lemma) For every graph $G = (V, E)$ it holds
  $
    sum_{u in V} d(v) = 2|E|
  $
]

#pf[Each edge is counted twice in the sum on the left-hand side, once for each incident vertex.] \
#cl(21.3)[In every graph, the number of vertices with odd degree is always even.] \
#pf[According to the handshaking lemma, the sum of all degrees is even. By subtracting the sum of all even degrees, an even number is obtained, which represents the sum of all odd degrees. Therefore, the sum of all odd degrees must have an even number of summands.]

#ex(
  21.4,
)[Can $333$ telephones be connected such that each telephone is directly connected to three other telephones? The answer is no, because the sum of the degrees of the telephone network would be odd $(333 dot 3)$.] \
The degree sequence of a graph $G$ is the decreasingly sorted sequence of the degrees of all vertices in $G$. For example, the graph in Fig. 21.1 has the degree sequence $(3, 2, 2, 1)$. Conversely, not every decreasing sequence of natural numbers corresponds to a graph. For example, there is no graph with degree sequence $(5, 3, 2, 2, 2, 1)$, because the sum of the degrees is odd.

=== Subgraphs

Let $G = (V, E)$ be a graph. A subgraph of $G$ is a graph $G′ = (V', E')$ with $V' ⊆ V$ and $E' ⊆ E$. A subgraph $G'$ of $G$ can be viewed as the subgraph of $G$ induced by its edge set $E'$. A subgraph $G'$ of $G$ is called the subgraph induced by its vertex set $V'$if every edge in $G$ connecting two vertices in $G'$ belongs to $G'$.

#ex(
  21.5,
)[In Fig. 21.2, a graph $G$ along with two of its subgraphs $G_1$ and $G_2$ is shown. The subgraph $G_2$ is induced by the vertex set ${b, c, d}$, while the subgraph $G_1$ is not.]

#figure("TODO", caption: [21.2. A graph $G$ with two subgraphs.])

=== Isomorphisms

Let $G = (V, E)$ and $G' = (V', E')$ be graphs. A mapping $ϕ : V → V'$ is called an isomorphism from $G$ to $G'$ if $ϕ$ is bijective and for all $u, v ∈ V$ it holds $u v in E$ if and only if $ϕ(u)ϕ(v) in E'$. Two graphs $G$ and $G'$ are called isomorphic if there exists an isomorphism from $G$ to $G'$.

#ex(
  21.6,
)[The two graphs in Fig. 21.3 are isomorphic, because an isomorphism is given by $ϕ(a) = 1$, $ϕ(b) = 2$, $ϕ(c) = 3$ and $ϕ(d) = 4$.]

Isomorphic graphs have the same number of vertices (order), the same number of edges (size), and the same degree sequence. In other words, two graphs are not isomorphic if they have different order, size, or degree sequence. There exist non-isomorphic graphs with the same degree sequence (Fig. 21.4).

#figure("TODO", caption: [21.3. Two isomorphic graphs.])

#figure("TODO", caption: [21.4. Two non-isomorphic graphs with the same degree sequence.])

Let $G$ be a graph. An isomorphism from $G$ to $G$ is also called an automorphism of $G$. \
#th(21.7)[The set of all automorphisms of a graph forms a group under composition.]
The group of all automorphisms of G is called the automorphism group of $G$ and denoted by $"Aut"(G)$. \
#ex(
  21.8,
)[The automorphism group of a square (Fig. 21.5) is the dihedral group D4 according to 15.30, consisting of four rotations $"id"$, $(1234)$, $(1234)2 = (13)(24)$, $(1234)3 = (1432)$ and four reflections $(12)(34)$, $(14)(23)$, $(13)$, $(24)$.]

== Paths, Cycles and Connectivity

Let $G = (V, E)$ be a graph. A sequence $W = (v_0, dots, v_k)$ of vertices $v_i ∈ V$ is called a path in $G$ if for all $i, 1 ≤ i ≤ k$, it holds $v_(i−1)v_i ∈ E$. The vertex $v_0$ is the starting vertex and the vertex $v_k$ is the ending vertex of $W$. The length of $W$ is $n$, the number of its edges. A path $W$ is called simple if $W$ does not contain any vertex more than once. \
#ex(
  21.9,
)[The graph in Fig. 21.6 contains, for example, the simple paths $(s, a, d, g, f, i, t)$ and $(s, a, b, e, h, i, t)$ of length $6$.] \

A cycle in $G$ is a path in G in which the starting and ending vertices are identical. A cycle is called simple if it does not contain any vertex more than once (except for the starting and ending vertex). A back-and-forth traversal of an edge $u v$ results in a simple cycle $(u, v, u)$ of length $2$. \
#ex(
  21.10,
)[The graph in Fig. 21.6 contains, for example, the simple cycles $(a, b, c, h, e, d, a)$ and $(a, b, e, f, g, d, a)$ of length $6$.]

=== Connectivity

Let $G = (V, E)$ be a graph. Two vertices $u, v ∈ V$ are called connected in G, briefly $u equiv_G v$, if u = v or there exists a path from u to v. If every pair of vertices in G is connected, then G is called connected. \
#lm(21.11)[Let G = (V, E) be a graph. The connectivity $equiv_G$ in $G$ is an equivalence relation on V.]

The equivalence classes of connectivity form, according to Theorem 5.5, a partition of V . The subgraphs spanned by the equivalence classes are called components of G. If G is connected, then there is only one component.

#figure("TODO", caption: [Fig. 21.6. A road network])

#ex(
  21.12,
)[The graph in Fig. 21.7 consists of two components, spanned by the equivalence classes {a, b} and {c, d, e}.] \
#th(
  21.13,
)[Let G = (V, E) be a connected graph and e ∈ E an edge lying on a simple cycle in G. The subgraph of G obtained by removing e is also connected.] \
#pf[Let G′ denote the subgraph of G obtained by removing e. Let $u, v ∈ V$. Since G is connected, there exists a path W in G from u to v. If the path does not use the edge e, then W is also a path in G'. Otherwise, W can be modified so that instead of the edge e, the rest of the cycle on which e lies according to assumption is used.]

=== Distances

Let $G = (V, E)$ be a graph. For any two vertices $u, v ∈ V$, the distance $d_(G)(u, v)$ between u and v in G is defined by
$
  d_(G)(u, v) =
  cases(
    0 & "if" u = v,
    infinity & "if" u "and" v "are not connected",
    l & "if" l "is the minimal length of a path in" G "from" u "to" v
  )
$
#th(21.14)[If G is a graph, then the distance $d_G$ defines a metric on G.] \
#pf[We only show the triangle inequality. Let u, v, w ∈ V . From shortest paths between u and v as well as v and w, a path of length $d_(G)(u, v) + d_(G)(v, w)$ between $u$ and $w$ is obtained. For the shortest path between $u$ and $w$, it holds by definition $d_(G)(u, w) ≤ d_(G)(u, v) + d_(G)(v, w)$.]

#figure("TODO", caption: [21.7. A graph with two components.])

=== Trees

A graph is called acyclic if it contains no simple cycle of length ≥ 3. Simple cycles of length 2 are described by back-and-forth traversals of edges and exist in every graph with at least one edge. An acyclic graph is called a forest. A connected forest is called a tree (Fig. 21.8).

#figure("TODO", caption: [21.8. A forest with two trees.])

#th(21.15)[A tree contains at least two vertices of degree 1.] \
#pf[Let G be a graph. Let vertices u and v in G be chosen such that the distance $d_(G)(u, v)$ is maximal. Let $W = (u, v_1, dots,v_(k−1), v)$ be a shortest path in G from u to v. Assume that u has another adjacent vertex w besides v1. Then by assumption d_(G)(w, v) ≤ d_(G)(u, v). Thus, there exists a shortest path from w to v that does not pass through u. Consequently, there is a contradiction, as there would be a simple cycle of length ≥ 3 in G. Thus, u has degree 1 and by symmetry v also has degree 1.] \
#th(21.16)[For every tree $G = (V, E)$ it holds $|E| = |V| − 1$.] \
#pf[The case $|V| = 1$ is clear. Let G be a tree with $|V| > 1$ vertices. By Theorem 21.15, there exists a vertex $v$ of degree $1$ in $G$. By removing v, a subgraph $G′ = (V', E')$ of $G$ is obtained, which is also a tree. By the induction hypothesis, we get $1 = |V'| − |E'| = (|V| − 1) − (|E| − 1) = |V| − |E|$.]

Let $G = (V, E)$ be a graph. A spanning tree or spanning forest of $G$ is a subgraph of $G$ that is a tree and contains every vertex of $G$ (Fig. 21.9). \
#th(21.17)[Every connected graph has a spanning tree.] \
#pf[Let $G = (V, E)$ be a connected graph. In the case $|E| = 1$, the statement is correct. Let $|E| > 1$. If $G$ is a tree, then $G$ is its own spanning tree. Otherwise, there exists a simple cycle in $G$. In this cycle, an arbitrary edge is removed. The resulting subgraph $G'$ of $G$ has $|E|−1$ edges and is connected by Theorem 21.13. Thus, $G'$ has a spanning tree by the induction hypothesis. This spanning tree is also a spanning tree of $G$.]

#figure("TODO", caption: [21.9. A spanning tree for the graph in Fig. 21.6.])

#th(21.18)[A connected graph $G = (V, E)$ is a tree if and only if $|E| = |V| − 1$.] \
#pf[Let $|E| = |V| − 1$. Assume that $G$ is not a tree. Then there exists a simple cycle in $G$. In this cycle, an arbitrary edge is removed. The resulting subgraph $G'= (V, E')$ of $G$ is connected by Theorem 21.13. For the edge set of G', it holds $|E'| < |V| − 1$. On the other hand, $G'$ contains a spanning tree with $|V| − 1$ edges by Theorems 21.16 and 21.17, whose edges contradictingly lie in $E'$. The converse has already been proven in Theorem 21.16.]

An edge $e$ of a graph $G$ is called a bridge or isthmus of $G$ if $G$ splits into two components by removing $e$. For example, the graph in Fig. 21.1 has the edge $v_1v_3$ as its only bridge.

=== Bipartite Graphs

A graph $G = (V, E)$ is called bipartite if there exists a 2-partition of $V$ into subsets $V_1$ and $V_2$ such that each edge in $G$ has one endpoint in $V_1$ and one endpoint in $V_2$.

#figure("TODO", caption: [21.10. A bipartite graph with the 2-partition {{a, b, c}, {d, e}}.])

#th(21.19)[A connected graph $G$ is bipartite if and only if $G$ contains no cycle of odd length.] \
#pf[Let $G = (V, E)$ be bipartite with the 2-partition ${V_1, V_2}$. Let $K = (v_0, v_1, dots, v_k)$ be a cycle in $G$. Without loss of generality, let $v_0 ∈ V_1$. Then $v_1 ∈ V_2$, $v_2 ∈ V_1$, $v_3 ∈ V_2$, etc. Thus, $v_k = v_0 ∈ V_1$. Hence, $K$ has even length. \
  Conversely, suppose G contains no cycle of odd length. Let v ∈ V . We define
  $
    V_1 = {u ∈ V | d_(G)(v, u) ≡ 1 "mod" 2}
  $
  and
  $
    V_2 = {u ∈ V | d_(G)(v, u) ≡ 0 "mod" 2} .
  $
  It is clear that ${V_1, V_2}$ is a 2-partition of $V$ . Assume there exists an edge $u w$ in $G$ with $u, w ∈ V_1$. Then there is a cycle consisting of the edge $u w$, a path of length $d_(G)(w, v)$ from $w$ to $v$, and a path of length $d_(G)(v, u)$ from $v$ to $u$. This cycle has length $1 + d_(G)(w, v) + d_(G)(v, u)$, which is contradictingly odd by assumption. Analogously, it is shown that there is no edge $u v$ in $G$ with $u, v ∈ V_2$.]

== Planar Graphs

A graph is called planar if it has a diagram that can be drawn in the plane without crossings. Such a diagram is called plane. A plane diagram divides the drawing plane into faces or regions, where the drawn edges are considered as boundary lines. \
#ex(
  21.20,
)[The hexahedron has a plane diagram that divides the drawing plane into six faces (Fig. 21.11). The region outside the plane diagram is also counted.]

#figure("TODO", caption: [21.11. A hexahedron and a plane diagram of the hexahedron (with faces $A, dots, F)$.])

#th(
  21.21,
)[(Euler’s polyhedron formula) For every plane diagram of a connected graph $G$ with $n$ vertices, $m$ edges, and $f$ faces it holds]
$
  n − m + f = 2
$

#pf[In the case $f = 1$, $G$ has a plane diagram with one face. Thus, $G$ is acyclic, hence a tree. By Theorem 21.16, $m = n − 1$, from which the claim follows. \
  Let $f ≥ 2$. Then $G$ contains at least one simple cycle. If an edge lying on this cycle is removed, the resulting graph $G'$ has $m − 1$ edges and $f − 1$ faces. By the induction hypothesis, $n − (m − 1) + (f − 1) = 2$, hence $n − m + f = 2$.]

#th(21.22)[A planar connected graph $G$ with $n ≥ 3$ vertices has at most $3n − 6$ edges.] \
#pf[Let $D$ be a plane diagram of $G = (V, E)$ and $F$ the set of all faces in $D$. We consider a relation $R$ between edges and faces. Let $e R f$ if in $D$ the edge $e$ bounds the face $f$. Each edge bounds at most two faces and each face is bounded by at least three edges. By the principle of double counting, $3|F| ≤ 2|E|$. From this, Euler’s polyhedron formula yields $|E| ≤ 3n − 6$.]

A graph $G$ is called complete if every two vertices in $G$ are adjacent. A complete graph with n vertices has $binom(n, 2)$ edges and is denoted by $K_n$ (Fig. 21.12). The smallest non-planar complete graph is $K_5$. This is because this graph has $10$ edges, while every planar graph with five vertices has at most $3 dot 5 − 6 = 9$ edges according to Theorem 21.22. The girth of a graph $G$ is the length of the shortest simple cycle in $G$. The girth of an acyclic graph is defined as $infinity$.

#figure("TODO", caption: [21.12. The first five complete graphs K1, . . . , K5.])

#th(
  21.23,
)[A planar connected graph $G$ with $n ≥ 3$ vertices and girth $g ≥ 3$ has at most $max(g(n−2)/(g−2), n−1)$ edges.] \
#pf[If $G$ is acyclic, then $g > n$ and the maximum is $|E| ≤ n − 1$, which is correct according to Theorem 21.18. Otherwise, $g ≤ n$. We distinguish two cases. First, suppose $G$ has a bridge. Then $G$ splits into components $G_1$ and $G_2$ by removing the bridge. Let $G_i$ have $n_i$ vertices and $m_i$ edges. By the induction hypothesis, we get
  $
    m & = m_1 + m_2 + 1 \
      & ≤ max(g(n_1−2) /(g−2), n_1−1) + max(g(n_2−2) /(g−2), n_2−1) \
      & ≤ max(g(n−2)/(g−2), n−1)
  $
  Second, suppose $G$ is bridgeless. Let $f_i$ denote the number of faces in $G$ bounded by $i$ edges. Then
  $
    2m = sum_i i f_i = sum_(i gt.eq g) i f_i = g f
  $
  With Euler’s polyhedron formula, we get
  $
    m+2 = n+f lt.eq n+2/g m,
  $
  from which $m lt.eq g(n−2)/(g−2)$ follows]

A bipartite graph is called complete if for the 2-partition ${V_1, V_2}$ of its vertex set it holds that every vertex in $V_1$ is connected to every vertex in $V_2$. \
A complete bipartite graph with $|V_1| = m$ and $|V_2| = n$ is denoted by $K_(m,n)$. \
The complete bipartite graph $K_(3,3)$ is not planar (Fig. 21.13). This is because this graph has $9$ edges, while every planar graph with six vertices has at most $max(4(6−2)/(4−2), 6−2) = 8$ edges according to Theorem 21.23.

#figure("TODO", caption: [21.13. The bipartite graph $K_(3,3)$.])

We establish a necessary and sufficient criterion for the planarity of graphs. \
A subdivision of a graph $G$ is a graph obtained from $G$ by successively applying the following operation: Choose an edge $u v$ in $G$ and replace it by a path $(u, w_1, dots, w_k, v)$, where the vertices $w_i, 1 ≤ i ≤ k$, do not appear in $G$. Two graphs are called homeomorphic if they are subdivisions of the same graph (Fig. 21.14).

#figure("TODO", caption: [21.14. A graph and two subdivisions of this graph])

#th(
  21.24,
)[(Kurt Kuratowski, 1896-1980) A graph is planar if and only if it contains no subgraph homeomorphic to $K_5$ or $K_(3,3)$.] \
#ex(21.25)[The graph in Fig. 21.15 is not planar because it contains a subgraph homeomorphic to $K_(3,3)$]

#figure("TODO", caption: [21.15. A non-planar graph containing a subdivision of $K_(3,3)$.])

== Data Structures and Algorithms

Let $G = (V, E)$ be a graph with $V = {v_1, dots, v_n}$. The adjacency matrix of $G$ is an $n times n$ matrix $A(G) = (a_(i j))$ with
$
  a_(i j) = cases(
    1 "if" v_i v_j in E,
    0 "otherwise"
  )
$
For example, the graph in Fig. 21.1 has the adjacency matrix
$
  A = mat(
    0, 0, 1, 0;
    0, 0, 1, 1;
    1, 1, 0, 1;
    0, 1, 1, 0;
  )
$

#th(
  21.26,
)[Let $A$ be the adjacency matrix of a graph $G$. The $(i, j)$-th entry of the $k$-th power of $A$ gives the number of paths in $G$ of length $k$ from $v_i$ to $v_j$.] \
#pf[We set $A^k =(a^((k))_(i j))$. For k = 1, the statement is clear. For k ≥ 1, by definition
  $
    a^((k+1))_(i j) = sum_(l=1)^n a^((k))_(i l) a_(l j)
  $
  where $a^((k))_(i j)$ is the number of paths in $G$ of length $k$ from $v_i$ to $v_j$ by the induction hypothesis. The term $sum_(l=1)^n a^((k))_(i l) a_(l j)$ describes the number of paths in $G$ of length $k+1$ from $v_i$ to $v_j$ that consist of a path of length $k$ from $v_i$ to $v_l$ and an edge $v_l v_j$ , summed over all appropriate intermediate vertices. No other paths of length $k$ from $v_i$ to $v_j$ exist. Thus, the statement is proven.]


Let $G = (V, E)$ be a graph with $V = {v_1, . . . , v_n}$ and $E = {e_1, . . . , e_m}$. The incidence matrix of $G$ is an $n times m$ matrix $B(G) = (b_(i j))$ with
$
  b_(i j) = cases(1 "if" v_i "is incident with" e_j, 0 "otherwise")
$
For example, the graph in Fig. 21.1 has the incidence matrix $(e_1 = v_1v_3, e_2 = v_2v_3, e_3 = v_2v_4, e_4 = v_3v_4)$
$
  B = mat(
    1, 0, 0, 0;
    0, 1, 1, 0;
    1, 1, 0, 1;
    0, 0, 1, 1;
  )
$

An incidence matrix contains exactly two ones in each column and in each row as many ones as the degree of the corresponding vertex indicates.

A precursor to the matrix representation of graphs is the representation by lists. An adjacency list of a graph $G = (V, E)$ with vertex set $V = {v_1, . . . , v_n}$ consists of lists $L_1, . . . , L_n$, where list $L_i$ contains the vertices adjacent to $v_i$. The graph in Fig. 21.1 is represented by the following adjacency list: $L_1 : v_3$, $L_2 :v_3, v_4$, $L_3 : v_1, v_2, v_4$ and $L_4 : v_2, v_4$.

The storage requirement (number of storage cells) for a graph $G = (V, E)$ depends on the data structure in which it is stored. The storage requirement for an adjacency matrix is $O(|V|^2)$, for an incidence matrix $O(|V||E|)$ and for an adjacency list $O(|V|+|E|)$. An algorithm for a graph $G$ must read at least the vertices and edges of $G$. For this, the time requirement (number of computational operations) is $O(|V|+|E|)$. Therefore, a graph algorithm with linear time requirement $O(|V|+|E|)$ must represent the graph through an adjacency list (or a similarly space-efficient data structure).

We present two algorithms that construct a spanning tree of a graph. Let $G = (V, E)$ be a connected graph, $v^+$ the set of vertices adjacent to $v ∈ V$ and $v_0 ∈ V$ fixed. Algorithm 21.1 uses a list $L$ where elements are appended at the end and removed from the front. The vertices of the graph are traversed in breadth, i.e., after each vertex, all its adjacent vertices are processed first. This is managed by a list organized as a queue ("first-in, first-out"). This search principle is called breadth-first search.

Algorithm 21.2 uses a list $L$ accessible only at one end. The vertices of the graph are traversed in depth, i.e., after each vertex, the adjacent vertices of its adjacent vertices are processed. This is managed by a list organized as a stack ("last-in, first-out"). This search principle is called depth-first search. If the graph is represented by an adjacency list, the running time of both algorithms is $O(|V|+|E|)$. This is because in both cases the adjacency list is processed exactly once (Fig. 21.16).

#algorithm-figure("BreadthSearch(G)", vstroke: .5pt + luma(200), {
  import algorithmic: *
  Function(
    "BreadthSearch",
    [$G$],
    {
      ([choose starting vertex $v_0 in V$],)
      ($L := {v_0}$,)
      ($V' := {v_0}$,)
      ($E' := emptyset$,)
      While(
        $L eq.not emptyset$,
        {
          ([Remove the first vertex $v$ from $L$],)
          ([*print* $v$],)
          For([*all* $w in v^+$ and $w$ has not yet been in $L$], {
            If($w in.not V'$, {
              ($V' := V' union {w}$,)
              ($E' := E' union {v w}$,)
              ([append $w$ to the end of $L$],)
            })
          })
        },
      )
    },
  )
})

#algorithm-figure("DepthSearch(G)", vstroke: .5pt + luma(200), {
  import algorithmic: *
  Function(
    "DepthSearch",
    [$G$],
    {
      ([choose starting vertex $v_0 in V$],)
      ($L := {v_0}$,)
      ($V' := {v_0}$,)
      ($E' := emptyset$,)
      While(
        $L eq.not emptyset$,
        {
          ([$v in L$ is the top element of the list],)
          IfElseChain(
            [there exist $w in v^+$ and $w$ has ot yet been in $L$ then],
            {
              ([insert $w$ into $L$],)
              ($V' := V' union {w}$,)
              ($E' := E' union {v w}$,)
            },
            {
              [remove $v$ from $L$]
              [*print* $v$]
            },
          )
        },
      )
    },
  )
})
})
=== Self-test Problems

= Networks

== Shortes Paths

=== Shortest Paths Between Any Two Vertices

=== Shortest Paths from One to All Others

== Minimum Spanning Trees

=== Kruskal's Algorithm

=== A Criterion for Minimum Spanning Trees

== Maximum Flows

=== Flows in Flow Networks

=== Cuts in Flow Networks

== Theorems of Hall, König-Egerváry and Menger

=== The Theorem of Menger

=== The theorem of König and Egerváry

=== The Theorem of Hall

=== Self-test Problems

= Combinatorial Optimization

== Complexity Classes

=== Complexity of Decision Problems

==== Complexity of Optimization Problems

== Backtracking Algorithms

=== Combinatorial Optimization

=== Backtracking Procedures

=== Pruning Subtrees

=== Bounding Functions

=== The Traveling Salesman Problem

== Heuristic Algorithms

=== The Hill-Climbing Method

=== Simulated Annealing

=== Genetic Algorithms

== Greedy Algorithms and Matroids

=== Greedy Algorithms

=== Matroids

== Vertex Coloring

=== Self-test Problems

= Linear Optimization

== Examples

=== Knapsack Problem

=== Matching Problem

=== Network Problem

== Linear Programs and Duality

=== Linear Programs

== The Feasible Region and the Role of Vertices

== Integer Programming

== The Simplex Method

=== Disclaimer

This automated translation is provided "as is" without warranties of any kind. The translator and provider make no representations about the accuracy, completeness, or suitability of the translation for any purpose. Use of this material is at your own risk. The original work remains under copyright protection, and this translation is provided for non-commercial academic use only.
