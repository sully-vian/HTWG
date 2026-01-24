#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  "local",
  "starting point",
  "local maximum",
  "convex",
  "heated and cooled",
  "upward movements",
  "temperature",
  "local",
  "cooling plan",
  "genetic algorithms",
  "biological evolution",
  "population",
  "selection",
  "mutation",
  "crossover",
  "optimal improvement",
  "short-sightedness",
  "a matroid",
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE09: Heuristic and greedy optimization])
#set par(justify: true)

#title()

Hill climbing is a generic uphill optimization method and belongs to the category of #word optimization techniques. A characteristic feature of this algorithm is that the result found is highly dependent on the chosen #word. Since it only searches for improvements in the immediate vicinity, it typically yields a #word Such methods are particularly effective for #word optimization problems, as these have only a single optimum.

To avoid the problem of getting stuck in a local optimum, simulated annealing can be used. This method is modeled on an industrial process in which a solid is #word. Unlike hill climbing, this algorithm allows for downward movements as well as #word. The probability of such a movement depends on the current #word. Since the method can leave a local optimum, it is classified as a #word optimization method. The temperature is reduced stepwise according to a so-called #word.

Another global approach involves #word which operate on the model of #word. Instead of managing a single solution, they manage a set of solutions called #word. The process comprises three essential steps: #word individuals are chosen based on their fitness; #word where an individual is replaced by a neighboring one; and #word pairs are combined to produce new offspring.

Finally, there is the group of greedy algorithms, which includes Kruskal's algorithm. These select the currently #word. However, due to their #word they often end up in a local optimum. A greedy algorithm is only guaranteed to be optimal if the underlying problem can be characterized by #word.
