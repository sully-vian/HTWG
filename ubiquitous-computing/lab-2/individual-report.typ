#set document(author: "Vianney Hervy", title: [Ubiquitous Computing - Lab 2: Node-RED])
#set page(numbering: "1")
#set heading(numbering: "1.")

#title()

Vianney Hervy

= Exercise 1

#image("assets/exercise-1.png")

This flow contains two nodes: _helloWorld_ and _Debug_. _helloWorld_ is an inject node that has a string payload of value `Hello World`. _Debug_ is a debug node that has an output set to `msg.payload` in order to output the payload received from _helloWorld_.

= Exercise 2

#image("assets/exercise-2.png")

This flow contains an additional _Change_ node to the ones of the Exercise 1. _Change_ is a change node which has a Change rule with `msg.payload` as input, searches for `World` and replaces with `Mars`.

= Exercise 3

#image("assets/exercise-3.png")

This flow is made with the `>= 6` rule in the _Switch_ node. The top debug node outputs the earthquake data (line per line) at every injection. The bottom debug node outputs `PANIC!` for every earthquake that has a mag(nitude) superior or equal to 6.

= Exercise 4

#image("assets/exercise-4.png")

This flow uses two "Date/Time Formatter" to format the raw epoch of the inject node. To get the short format of "Date Time Format 1", we should specify `DD/MM/YY HH:mm:ss` as output format. To get the longer format of "Date Time Format 2", we shoud specify `dddd, MMMM Do YYYY, h:mm:ss a` as output format.

I'm not sure I get what the "filter" node is used for in this flow, so I just used the default configuration.

Inputing the dates into "text" nodes of the \@flowfuse/node-red-dashboard collection allows to us to display the results in the following dashboard:

#image("assets/exercise-4-bis.png")

Of course, the debug nodes also print the displayed content in the debug window:

#image("assets/exercise-4-ter.png")

= Exercise 5

#image("assets/exercise-4-bis.png")

