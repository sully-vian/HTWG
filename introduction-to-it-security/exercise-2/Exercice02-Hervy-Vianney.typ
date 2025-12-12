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

#let source(name, year, pages) = {
  let pages = pages.map(p => str(p)).join(", ")
  footnote[*#name*, #year, p. #pages]
}


#set document(author: "Vianney Hervy", title: [Introduction to IT Security - Exercise sheet 2])
#set page(numbering: "1")
#set heading(numbering: (n1, ..x) => numbering("1.", n1 - int(n1 != 0), ..x))
#set par(justify: true, first-line-indent: (amount: 1em, all: true))

#title()

Vianney HERVY

= Use of generative AI

For this report, I used Google's Gemini#footnote[https://gemini.google.com/] exclusively for halping translating certain sentences in the provided sources in german. Not only is it better at reformulating in a more english/french-native way, but is also able to simplify and sumarize the statements.

= Current Attacks#footnote[https://www.news.admin.ch/]

The Schweizerische Gesundheitsstiftung RADIX (Radix Foundation) is a non-profit organization in Switzerland that acts as a national competence center for the development and implementation of public health and sustainable development measures. As it provides services and handles sensitive data for various administrative units of the Swiss Federal Administration and public health initiatives, it represents a critical point in the nation's supply chain.

The attack was carried out by a known cybercrime organization: the Sarcoma ransomware group. Their primary goal was financial extortion: Demanding a ransom payment from Radix in exchange for a decryption key to restore encrypted data. The attackers also stole 1.3 TB of sensitive data before encryption. Their secondary goal was to threaten the company to leak this stolen data. Ultimately, Radix refused to meet the ransom demands, leading to the successful double extortion where the stolen 1.3 TB of sensitive data was leaked, resulting in significant reputational and legal damage.

Radix did not communicate precisely on its security mechanisms but we can assume that working closely with the Swiss Federal Administration offices and handling sensitive public health information requires high security standards. How the malware overcame the security mechanisms can also only be inferred.

Radix's major security mechanism is data backup. Backuping all data voids any random attack and prevents data losses. Radix's network is segmented in different infrastructures. This measure makes any attack less scalable because it may isolate the critical services from the compromised network. Concerning reactivity mechnisms, Radix's team immediately revoked access to the compromised systems and engaged the National Cyber Security Centre, Zurich Police and Data Protection Commissioner. Fast response is often what limits the damages.

Despite the presumed high standards, the Sarcoma group’s success implies that the attackers effectively bypassed the initial perimeter and then moved laterally to the main network. While the specific exploit remains undisclosed, the initial access was likely achieved through a spear phishing campaign or by exploiting a vulnerability in a publicly accessible service (like a VPN). This initial foothold overcame the perimeter defense. Once inside, the attackers probably exploited poor credential hygiene to escalate privileges from a basic user account to a domain administrator. With elevated rights, the attackers disabled or avoided endpoint detection tools before initiating their large-scale data exfiltration—bypassing logging and internal network monitoring. Crucially, while network segmentation successfully protected the separate counseling services, it failed to fully isolate the main administrative and data storage networks, allowing the threat actors to spread widely enough to encrypt key systems and steal 1.3 TB of data before the IR team could fully contain the main breach.

= Malware

== Number of new Malwares reported daily#source("Cybercrime Bundeslagebild", 2019, (15,))

Approximately 11.500 new mobile malware variants are registered daily, with Android devices being the primary target.

== Botnets#source("Cybercrime Bundeslagebild", 2019, (3, 25, 39))

A botnet is a network of computers (roBOT-NETwork) that have been secretely infected with malware. These devices are controlled by command servers and are frequently used for DDoS attacks. Internet of Things is playing a large role as botnet component. Indeed, there is a growing number of devices to infect and control.

Android is the primary target for mobile malware, which is often used to establish or expand botnets for mobile devices. Most of the IoT devices run on Android, restating the point previously made.

== Typical actions perfomed before executing ransomware#source("Die Lage der IT-sicherheit in Deutschland", 2022, (15, "(figure 3)"))

The attack begins with an initial infection where a spam email with a malicious attachment compromises a victim's device. Once inside, the attackers perform lateral movement to navigate from the single infected machine into the broader corporate network. Finally, before locking any files, they steal sensitive data and exfiltrate it to an external server, ensuring they have leverage for extortion before the encryption phase begins.

== Recommended mechanisms againts malware#source("IT-Grundschutz-Compendium", 2022, ())

#table(
  columns: (auto, auto, auto),
  align: horizon,
  table.header([*ID*], [*Requirement Title*], [*Protection Level*]),
  [OPS1.1.4], [Creating a Malware Protection Concept], [Basic],
  [OPS.1.1.4.A2], [Using System-Specific Protection Mechanisms], [Basic],
  [OPS.1.1.4.A3], [Selection of a Virus Protection Program], [Basic],
  [OPS.1.1.4.A5], [Operation and Configuration of Virus Protection Programs], [Basic],
  [OPS.1.1.4.A6], [Regular Updating of Virus Protection Programs and Signatures], [Basic],
  [OPS.1.1.4.A7], [User Awareness and Obligations], [Basic],
  [OPS.1.1.4.A9], [Reporting Malware Infections], [Standard],
  [OPS.1.1.4.A10], [Using Special Analysis Environments], [Increased],
  [OPS.1.1.4.A11], [Using Several Scan Engines], [Increased],
  [OPS.1.1.4.A12], [Using Storage Media Locks], [Increased],
  [OPS.1.1.4.A13], [Handling Untrusted Files], [Increased],
  [OPS.1.1.4.A14], [Selecting and Using Cyber Security Products to Thwart Targeted Attacks], [Increased],
)

= Vulnerability Management

For this part, the IDs and descriptions and CVSS scores come directly from the EUVD's public API.

#let data = json("moodle-vulnerabilities.json")
#let assets = (
  [The Moodle database (containing all user, grades and course data],
  [Admin session/account (via cookie) and web server integrity],
  [The server filesystem (LaTeX could read `/etc/passwd` or the Moodle config)],
  [User accounts and session Tokens],
)
#let workarounds = (
  [No official config workaround. The vulnerability lies deep in the core search logic. However, disabling the feature will prevent any attack.],
  [Disabling Live log report],
  [Disabling the TeX notation filter],
  [Raise awareness among teachers about not clicking on suspicious links],
)

#for (item, asset, workaround) in data.items.zip(assets, workarounds) {
  let reference = item.references.split("\n").at(0)
  [
    == #item.id (#item.baseScore)#footnote[#reference]
    - *Description:* #item.description
    - *Assets:* #asset
    - *Workarounds:* #workaround
  ]
}

In most of the analyzed cases, the only effective workaround is to completely disable the affected feature. This highlights the critical importance of Security by Design. If a product is not built with security in mind from the start, IT operators are often left with no good options once it is shippedn ,forcing them to choose between keeping a service running insecurely or shutting it down entirely.
