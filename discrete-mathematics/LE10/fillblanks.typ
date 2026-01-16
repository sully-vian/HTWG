#let blank = [\_\_\_\_\_\_]
#let _wc = counter("fillblankword-counter")

#let init-fillblanks(words) = {
  _wc.update(0)
  (
    words: words.map(s => [*#s*]),
  )
}
