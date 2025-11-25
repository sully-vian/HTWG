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

#let sci(x) = {
  let b = calc.floor(calc.log(calc.abs(x)))
  let a = calc.round(x / calc.pow(10.0, b), digits: 2)
  return $#a dot 10^#b$
}


#set document(author: "Vianney Hervy", title: [Introduction to IT Security - Exercise sheet 1])
#set page(numbering: "1")
#set heading(numbering: (n1, ..x) => numbering("1.", n1 - int(n1 != 0), ..x))

#title()

Vianney HERVY

= Use of generative AI

This report was written in Typst#footnote[https://typst.app/], a LaTeX equivalent. To help me with the unfamiliar math syntax#footnote[https://typst.app/docs/reference/math/], symbols#footnote[https://typst.app/docs/reference/symbols/sym/] and the layout, I used Google's Gemini#footnote[https://gemini.google.com/]. I also cross-checked the facts by submitting my work along with the slides. Finally, I checked the spelling and formulations.

= User Authentication: Passwords

#let users = 5000
#let chars = 26
#let length = 14
#let poss = calc.pow(float(chars), float(length)) // possible passwords

According to the instructions, there are #chars possible characters for each one of the #length symbols of the password. This gives $#chars times dots times #chars = #chars^#length approx #sci(poss)$ different passwords.

#let rate = 40000
#let secs-per-day = 3600 * 24
#let try-per-day = rate * secs-per-day
#let days-if-bruteforce = poss / rate * 1 / secs-per-day

== Base case

To get a specific password, it would take the attacker #sci(poss) attempts (number of passwords).

He can make $#rate _"tries/s" times #secs-per-day _"s/day" approx #sci(try-per-day)$ tries per day.

It would then take him $(#sci(poss) _"tries") /(#sci(try-per-day) _"tries/day") approx #sci(days-if-bruteforce)$ days.

This result is the worst case, if the targeted password is tried last. On average, it would take him approximately half that time (same order of magnitude)

#let rate = 5000
#let ratio = 90%
#let popular = 1000
#let target = 90
#let vulnerables = float(users * ratio)
#let P = float(ratio / popular)
#let attempts = target / P
#let total-time = attempts / rate

== Irregular distribution

The answer here depends on the attacker's strategy. The optimal one is to only test the 1,000 popular passwords and get #repr(ratio) of the passwords in a fifth of a second. But having a correct password is not enough; the user has to match as well ("_The right key is nothing without the right lock_").

There are $#users times #repr(ratio) = #vulnerables$ vulnerable users.

Each attempt is a user-password pair. The probability that that specific user has that specific password from the popular list is:
$
  P("User is vulnerable") times P("User chose this password") = #repr(ratio) times 1 / #popular = #P
$
To get #target passwords, the attacker would have to do $#target / #P = #sci(attempts)$ attempts

Given a #rate rate, it would take him $(#attempts _"tries")/(#rate _"tries/s") = #total-time$ seconds to find a matching pair.

== Defense

#let P-fail = 60%
#let P-succ = 100% - P-fail
#let n = P-succ / (ratio / popular)

Let $P_"vulnerable"$ (resp. $P_"secure"$) be the probability that the targeted account is vulnerable (resp. secure) and $P^(\(n\))_"success|vulnerable"$ (resp. $P^(\(n\))_"success|secure"$) the probability to guess correctly a vulnerable (resp. secure) password in $n$ tries.

The total probability of success is:

$
  P^(\(n\))_"success" = (P_"secure" times P^(\(n\))_"success|secure") + (P_"vulnerable" times P^(\(n\))_"success|vulnerable")
$
replacing with the values found earlier, we get
$
  P^(\(n\))_"success" = ((1 - #repr(ratio)) times n / #chars^#length + (#repr(ratio) times n / #popular)
$
The chance of finding a "secure" password is negligible for the probable value of $n$ we will get. The equation simplifies as such:
$
  P^(\(n\))_"success" approx 0 + n times #float(ratio / popular)
$

We want
$
  P^(\(n\))_"success" lt.eq.slant 1 - #repr(P-fail)
$
thus
$
  n times #float(ratio / popular) lt.eq.slant #float(P-succ)
$
therefore
$
  n lt.eq.slant #float(100% - P-fail) / #float(ratio / popular) approx #calc.round(n, digits: 3)
$

But $n in NN$ so the highest acceptable value is $n = #calc.round(n)$ attempts before shutting down the auth mechanism for this user.

This value is pretty high and no regular user would need so many attempts, $n$ can be lowered to an eighth ($#calc.round(n / 8)$ attempts) to allow even safer authentication.

= Access Control

== Mobile Apps

*Pinterest*#footnote[https://www.pinterest.com/] is a social media where users can view, share, comment and save "pins". Pinterest requests access to the contacts (read), location (read), camera (read) and storage (read/write). \
*Too Good To Go*#footnote[https://www.toogoodtogo.com/] connects customers to restaurants and stores to save unsold food surplus. The app requires the user's location (read) and their calendar (write). \
*Bonjour RATP*#footnote[https://www.bonjour-ratp.fr/en/] is the official app of the Parisian public transportation company. It requires access to the user's location (read) and to their physical activity data (read)

All these permissions are grouped from the app's perspective, the access rights are associated with the subject (the app). This is a capability list. Mobile systems use capability lists because each app runs in its own sandbox and needs only small, explicit permissions instead of complex resource-based rules. Capabilities are simpler for users and developers, enforce least privilege cleanly, and avoid the overhead of maintaining global access control lists. This model scales better, is easier to secure, and matches how mobile OS APIs are designed.

== Role-based Access Control

Roles allow permission grouping to avoid granting the same permissions again and again. Inheritance is also permitted, enabling simpler permission distribution and management. Roles may also be parameterized or hierarchized.

In an application like Moodle, the users can be grouped by role with parameters: "student of [program]", "lecturer of [subject]", "head of [department]". The permissions can be direct functions of these parameters: student can only read a course he is taking, lecturer can only write a course he is giving etc.

= Application Whitelisting

1. #underline("Explain how application whitelisting works with WDAC (Windows Defender Application Control)")

  Windows Defender Application Control enforces application whitelisting by using a policy that defines trusted apps (via the publisher, package, etc). Every executable, script or driver is checked against this policy, allowing only verified (approved) apps to run and blocking all others. The philosophy is that an unknown app is considered potentially unsafe until proven otherwise.

2. #underline("Does WDAC restrict users/processes with respect to executing code ? How ?")

  Yes, WDAC restricts code execution by enforcing its policy on all user accounts and processes. When a user or a process attempts to run an app, WDAC checks if the whitelist allows the application. If it is not approved, the execution is denied.

3. #underline("Does WDAC restrict users/processes with respect to reading and writing files ? How ?")

  No, WDAC does not control read/write operations. Its scope is limited to the "execute" operation. However, it can prevent an application from running entirely, which indirectly prevents it from reading/writing files.

4. #underline("What are the different file rules that you can use with WDAC ?")

  WDAC supports publisher rules (verified via digital signatures), file hash rules (ensuring installed/downloaded content is identical to expected) and path rules (based on location in the file system).

5. #underline("What is the purpose of the audit mode in WDAC ?")

  WDAC Audit Mode is a mode where the policy is enforced only for logging. It does not block any application from running but it records events about what would have been blocked if the policy were enforced. It is like a "test playground" where to test rules and policies before applying them.

6. #underline(
    "How do you debug WDAC policies, i.e., how do you find out that they are effective and where are WDAC events logged ?",
  )

  Debugging WDAC policies relies on the Audit Mode to check effectiveness and identify missing policy rules. The WDAC events are systematically logged into the *Application and Services Logs\\Microsoft\\Windows\\CodeIntegrity\\Operational* event log.

