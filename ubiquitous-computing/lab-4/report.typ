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
#set par(first-line-indent: (amount: 1em, all: true), justify: true)

#title()

Vianney Hervy

= Installation

My laptop runs on Linux and other lectures have taught me Docker. This is why I chose the second provided option. I wrote and ran the following script:

#framed-code("bash", read("./run.sh"))

I had no issue starting the container or accessing the dashboard at the expected address #link("http://localhost:8123").

#image("./assets/screenshot-1.4.png")

= Adding a first Service/Device

This part was also pretty straight forward given the detailed instructions and the online documentation. The following result was immediately achieved:

#align(center, image("./assets/screenshot-accuweather.png", width: 50%))

I however noted that the AccuWeather website changed its structure and that the "My Apps" tab does not exist anymore but the API key can be found in the "Subscriptions & Keys" tab#footnote[https://developer.accuweather.com/subscriptions] instead.

= First automations

After trying numerous unavailable usernames, I created a Telegram bot called _\@ThisSurelyIsntTakenYetBot_#footnote[https://t.me/ThisSurelyIsntTakenYetBot] and connected it to the Home Assistance instance using the provided API key. Following a YouTube tutorial#footnote[https://www.youtube.com/watch?v=5gcdUMCxYAk], I then added my user chat ID to later use my account as target for automations.

#align(center, image("./assets/telegram-bot-config.png", width: 50%))

Here again, the subject wasn't up to date. The `telegram_bot` integration does not support YAML configuration anymore. Everything has been moved to the graphical interface.

#align(center, image("./assets/screenshot-no-telegram-config.png", width: 50%))

Following the instructions, I created the two described automations with the desired triggers and actions. The periodic trigger was tricky to configure since I had to guess that the expected value would be `/5` to obtain a trigger every 5 minutes.

#two("./assets/sun-notif.png", "./assets/5-min-notif.png")

For more details, here is the generated yaml corresponding to the two configured automations:

#framed-code("yaml", read("./config/automations.yaml"))

I didn't wait long enough to see the messages sent by the Sun Notif automation, but here is a screenshot of the periodic automation.

#align(center, image("./assets/screenshot-chat.jpg", width: 31%))

= Smart Home Planning

#image("./assets/floor-plan.svg")
Source#footnote[https://www.shelterbayliving.ca/floor-plan-d/]
