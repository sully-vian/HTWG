#let framed-code(lang, body) = [
  #line(length: 100%, stroke: 0.1pt)
  #raw(body, lang: lang)
  #line(length: 100%, stroke: 0.1pt)
]

#let cent(content) = align(center, content)
#let two(img-1, img-2) = stack(
  dir: ltr,
  spacing: 10%,
  align(horizon, image(img-1, width: 45%)),
  align(horizon, image(img-2, width: 45%)),
)

#set document(author: "Vianney Hervy", title: [Ubiquitous Computing - Lab 3: Node-RED])
#set page(numbering: "1")
#set heading(numbering: "1.")
#set par(first-line-indent: (amount: 1em, all: true))

#title()

Vianney Hervy

= Exercise 1

The Arduino code was not difficult to write but I had to think to organize the read and write functionnalities.

#framed-code("cpp", read("Exercise1/Exercise1.ino"))

The flow was trickier to craft. In particular, the configuration of the serial port. Indeed, the Arduino sketch was way too fast for the Node-RED flow to follow. Instead of reading the numbers one by one, it read large chunks of `000...0` and `111...1`. To solve this problem, I added a delay of 100ms and a "toggle-only output" in the Arduino sketch. I also configured the serial port to split the input in chunks of 1 character.

#two("Exercise1/flow.png", "Exercise1/serial-port.png")

I also configured the led node to map 1 (resp. 0) to the red (resp. gray) color, so that it looks turned on when the arduino button is pressed and turned off otherwise. The switch node was configured accordingly with string "1" when "On Payload" and "0" when "Off Payload"

#two("Exercise1/dashboard.png", "Exercise1/picture.jpg")

Another issue I ran into was a set of GND pins that weren’t working. I eventually discovered that using my fingers worked as a temporary ground, but only after spending time testing each component to figure out which one was actually defective.

= Exercise 2

This exercise was pretty straightforward since a similar Arduino sketch was required for the first lab session. I just changed the code for the captor to read the temperature as a float and see more subtle fluctuations on the dashboard's chart.

#framed-code("cpp", read("Exercise2/Exercise2.ino"))

The gauge node is configured as instructed in the subject with three different ranges.

#two("Exercise2/gauge.png", "Exercise2/chart.png")

Below is the final dashboard after a few minutes of heating then cooling the Arduino board to display changes.

#align(center, image("Exercise2/dashboard.png", width: 50%))

= Exercise 3

This exercise's goal is to use a third element in the information chain: cloud software. The data is collected by the Arduino board's sensor as in the previous exercise (same code and serial in node), It is then sent to the MQTT broker through a dedicated node. Another dedicated node reads the data from the MQTT broker and outputs it to the debug console.

All nodes are on the same Node-RED computer, but the flow is seperated in two distinct components that don't directly communicate. This means they could be running on two different instances anywhere in the world. For instance one would be in a patient's home and the other at a hospital or a data collection center.

#two("Exercise3/flow.png", "Exercise3/fields.png")

I believe everyone had an issue with the Datacake part of this exercise. Indeed, the MQTT integration service is now a paid plan feature.

#align(center, image("Exercise3/mqtt-integration.png", width: 70%))
