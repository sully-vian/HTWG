#let framed-code(lang, body) = [
  #line(length: 100%, stroke: 0.1pt)
  #raw(body, lang: lang)
  #line(length: 100%, stroke: 0.1pt)
]

#let two(img-1, img-2) = stack(
  dir: ltr,
  spacing: 10%,
  align(horizon, image(img-1, width: 45%)),
  align(horizon, image(img-2, width: 45%)),
)

#set document(author: "Vianney Hervy", title: [Ubiquitous Computing - Lab 4: Smart Home])
#set page(numbering: "1")
#set heading(numbering: "1.")
#set par(first-line-indent: (amount: 1em, all: true))

#title()

Vianney Hervy

