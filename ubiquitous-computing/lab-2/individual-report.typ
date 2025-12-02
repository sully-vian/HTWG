#set document(author: "Vianney Hervy", title: [Ubiquitous Computing - Lab 2: Node-RED])
#set page(numbering: "1")
#set heading(numbering: "1.")

#title()

Vianney Hervy

All the work done here (along with the flows as JSON) is available on my GitHub#footnote[#link("https://github.com/sully-vian/HTWG/tree/main/ubiquitous-computing/lab-2")].

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

#image("assets/exercise-5.png")

This flow is sensibly more complex asthe displayed information comes from two different sources: the timestamp and the Open-Meteo weather API#footnote[https://github.com/sully-vian/HTWG/tree/main/ubiquitous-computing/lab-2].

The weather flow goes as such:

- *Change node:* Set `msg.payload` to "Konstanz" (or the city of your choice)
- *HTTP request node:* Fetch the city GPS coordinates with the following url: `https://geocoding-api.open-meteo.com/v1/search?name={{payload}}&count=1`
- *Change node:* Pick up the fetched values and bring them to the message's payload's root for easier access.
- *HTTP request node:* Fetch the weather data with the acquired GPS coordinates: `https://api.open-meteo.com/v1/forecast?latitude={{payload.latitude}}&longitude={{payload.longitude}}&current=temperature_2m,relative_humidity_2m`
- *Dashboard nodes:* Displatch the fetched information to the different dashboard nodes.

#stack(
  dir: ltr,
  spacing: 10%,
  figure(image("assets/exercise-5-bis.png", width: 45%), caption: [Configuration of the second change node]),
  figure(
    image(
      "assets/exercise-5-ter.png",
      width: 45%,
    ),
    caption: [Configuration of the gauge node],
  ),
)

Concerning the "The weather in ..." sentence, I used a template node with the following template which I then plugged into a regular dashboard text node.

#image("assets/exercise-5-quater.png")

I didn't manage to find a service that provides textual information such as the "cloudiness" shown in the subject example.

The resulting dashboard after a few hours of gathering data (weather API kicked me out and hid the temperature gauge value):

#image("assets/exercise-5-quiquies.png")

