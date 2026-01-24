#let wc = counter("word-counter")

#let blank = [\_\_\_\_\_\_]

#let words = (
  // TODO
).map(
  s => [*#s*],
)

#let word = context {
  wc.step()
  words.at(wc.get().at(0))
}

#set document(title: [LE06: Networks and graph theory])

#title()

// TODO: translate from german or find english version

English version missing from Moddle, will translate later
