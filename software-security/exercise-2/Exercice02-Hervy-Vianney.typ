#let ulink(url, text: none) = if (text == none) {
  underline(link(url)[#url])
} else {
  underline(link(url)[#text])
}

#let framed-code(lang, body) = [
  #line(length: 100%, stroke: 0.1pt)
  #raw(body, lang: lang)
  #line(length: 100%, stroke: 0.1pt)
]

#let sql(text) = {
  raw(text, lang: "SQL")
}

#let gql(text) = {
  raw(text, lang: "graphql")
}

#set document(author: "Vianney Hervy", title: [Software Security - Exercice sheet 2])
#set page(numbering: "1")
#set heading(numbering: "1.")
#set par(justify: true, first-line-indent: (amount: 1em, all: true))

#title()

Vianney HERVY

= Web Application Vulnerabilites (Without Time Constraint)

I picked the niteCTF 2025#footnote[https://ctftime.org/event/2851] event.

#align(center, image("assets/scoreboard.png"))

== Database Reincursion

This challenge is in the "Web Exploitation" section. We are given a login page#footnote[https://database.chals.nitectf25.live/] with username and password field.

#align(center, image("assets/access-portal.png", width: 45%))

After some messing around I find that some inputs do not return "Invalid username or password", but "Input rejected by security filter". That happens specifically when my input contains `OR` or `--` both of which have meaning in SQL. I concluded there was a filter protecting the backend from SQL injection.

I looked up multiple other examples of SQL injection online until `' UNION SELECT 1,2,3/*` worked and brought me to the "Employee Directory" page.

#align(center, image("assets/employee-directory.png"))

There, I read "I heard Kiwi from Management has the passcode". Looking up "Kiwi" shows 4 employees but none in management. Maybe the SQL query has a `LIMIT 4`. Indeed, inputting `'AND` breaks the qury and shows SQL error: unrecognized token: `' ORDER BY id LIMIT 4`. This message reveals the database is SQLite, and the exact structure of the query: #sql("WHERE name = '<input>'ORDER BY id LIMIT 4").

Having no access to the `OR` keyword, I tried using `IN` as `'IN (0,1) /*`, giving this: #sql("WHERE name = ''IN (0,1) /*'ORDER BY id LIMIT 4"), effectively always being true (boolean is 0 or 1). But that only showed my the same users as the default page did. I needed to specify that I wanted the "Kiwi" user in the "Management" department: #sql("Kiwi' AND department=\"Management\"/*"). which makes the following request: #sql("WHERE name = 'Kiwi' AND department=\"Management\"/*'ORDER BY id LIMIT 4") that correctly shows the "Kiwi" user for Management with the admin passcode.

Entering the passcode shows the following new page:

#align(center, image("assets/admin-panel.png"))

Quick tries show that the input isn't protected by the filter anymore. I can use `' OR 1 /*` to get all the reports quarters. I just need to extend the query to add elements from other tables. #sql("' UNION SELECT * FROM metadata --") shows me interesting results:

#align(center, image("assets/metadata.png"))

I now know the column names of the other tables and I discovered a new CITADEL_ARCHIVE_2077 which seems important. I crafted this input: #sql("'UNION SELECT secrets,secrets,secrets,secrets FROM CITADEL_ARCHIVE_2077--") (secrets repeated 4 times to match the metadata table's column number). Unfortunately, this returns "Citadel SysSec: Query max length exceeded". I need to find a more concise query.

After looking up the SQL syntax again, I found out I can just write #sql("'UNION SELECT secrets,1,1,1 FROM CITADEL_ARCHIVE_2077--") which is short enough and finally shows the flag.

== Graph Grief#footnote[This challenge was solved in collaboration with another user, `vietrux`. Naturally, this section draws inspiration from his writeup: #ulink("https://freebee.v1.io.vn/blog/nite-ctf-2025-writeup")]

This Challenge is in the "Web Exploitation" section. The provided hint is "Legacy XML importer may trigger internal file utilities"

Upon accessing the website, it was clear that the backend runs on Node.js, as indicated by the `x-powered-by: Express` header. The site advertised "AetherCorp" GraphQL API services. I attempted to access `/graphql` and found it to be an Apollo Server, but introspection was blocked.

Since introspection was disabled, I sent invalid queries to trigger error messages that might reveal the schema. For example, querying #gql("{ usrs }") returned a suggestion: `Did you mean 'users' ?`. This allowed me to map out available fields: `users`, `profiles`, `orders`, `products`, and `node(id: ID!)`.

When querying `users`, I noticed a `role` field, with some users having the "admin" role. All IDs were base64 encoded, following the Relay-style global ID standard. Testing different IDs with the `node` query, I discovered a type called `secret`:

#framed-code("graphql", "{ node(id: \"c2VjcmV0OmZsYWc=\") { ... on secret { id flag } } }")

this returned

#framed-code("graphql", "{ \"data\": { \"node\": { \"id\": \"secret:flag\", \"flag\": null } } }")

The `secret:flag` node exists, but the flag value was null. The goal was to retrieve the actual flag value.

The hint suggested a legacy XML importer. I sent a request with `Content-Type: application/xml`:

#framed-code("xml", "<?xml version=\"1.0\"?><root>hello</root>")

The server echoed the text content, indicating XML parsing. I immediately considered XXE (XML External Entity) injection.

Basic XXE attempts were blocked:

- SYSTEM entities: "General SYSTEM entities are not allowed."
- Localhost access: "Localhost access via SYSTEM entity is not allowed."
- Only HTTP/HTTPS schemes were allowed.

I set up an external DTD server using Ngrok and tested if the server would fetch and parse external DTDs. It did, but the server also checked the content of the external DTD for blocked schemes like `file://`.

I bypassed the filter by using an absolute path (without `file://`) in the external DTD:

#framed-code("xml", "
<!ENTITY % file SYSTEM \"/etc/passwd\">
<!ENTITY content \"%file;\">
")

This worked ! It allowed me to read `/etc/passwd`. I then located the flag by reading `flag.txt`, which contained the flag.

== Floating-Point Guardian

This challenge is in the "AI" section. We are given a tcp connection#footnote[ncat --ssl floating.chals.nitectf25.live 1337] and the source of the code running on that server.

The program asks multiple questions that can be answered with a number (height, age, heart rate...) and writes these in an array. The array is then passed through a neural network. The output is compared to a secret target value. The goal is to approach that target value as close as possible.

#framed-code(
  "",
  "$ ncat --ssl floating.chals.nitectf25.live 1337
I am the AI Gatekeeper.
Enter your details so I know you are my Master.
Answer these questions with EXACT precision...

[Q1]  What is your height in centimeters? 1
1
[Q2]  What is your weight in kilograms? 1
1
[Q3]  What is your age in years? 1
1
[Q4]  What is your heart rate (bpm)? 1
11
[Q5]  How many hours do you sleep per night?
11
[Q6]  What is your body temperature in Celsius?
11
[Q7]  How many steps do you walk per day?
11
[Q8]  What is your systolic blood pressure?
11
[Q9]  How many calories do you consume daily?
11
[Q10] What is your BMI (Body Mass Index)?
11
[Q11] How many liters of water do you drink daily?
11
[Q12] What is your resting metabolic rate (kcal/day)?
11
[Q13] How many hours do you exercise per week?
11
[Q14] What is your blood glucose level (mg/dL)?
11
[Q15] Rate this CTF challenge out of 10:
11


Processing through neural network layers...
========================================
MASTER PROBABILITY: 0.9939367441
========================================

You are NOT the Master.
The neural network has rejected your identity.
",
)

The input is a 1x14 vector, so bruteforcing or groping towards the solution is out of the question.

Given that we have the source code, we can reproduce the neural network and optimize the input to minimize the offset. That's what I did, I ported the code from C to Python and used `differenctial_evolution` from `scipy.optimize` to bring the result's offset to $10^(-11)$.

I then wrote the results to the tcp connection which recognised me as the master and sent the challenge's flag.

= Known Real-World Software Vulnerabilities

TODO

